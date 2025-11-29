#include "DisplayManager.h"

std::vector<Display*>* DisplayManager::getDisplays(DisplayBitmask displayBitmask) {
    displaysToApply.clear();
    for(Display* d : displays) {
        if(d->getBitmask() & displayBitmask) displaysToApply.push_back(d);
    }
    return &displaysToApply;
}

DisplayManager::DisplayManager(Matrix* matrix, HeadsUpDisplay* hud, bool enabled) {
    this->hud = hud;
    this->matrix = matrix;
    this->enabled = enabled;
    keyframe = nullptr;
    redrawRequested = false;

    effectBuffer.reserve(64);
    overlays.reserve(32);
}

Display* DisplayManager::addDisplay(Display* display) {
    if(getDisplayByBitmask(display->getBitmask())) {
        TOASTER_LOG("ERROR: Display with bitmask %d is already registered!\n", display->getBitmask());
        return nullptr;
    }
    if(getDisplayByPosition(display->getPosition())) {
        TOASTER_LOG("ERROR: Display at position %d is already registered!\n", display->getPosition());
        return nullptr;
    }
    displays.push_back(display);
    TOASTER_LOG(
        "Display %d registered [Position: %d, Size: %d, Bitmask: %d]\n", displays.size() - 1, display->getPosition(),
        display->getSize(), display->getBitmask()
    );
    return display;
}

void DisplayManager::addDisplays(std::initializer_list<Display*> displays) {
    for(Display* d : displays) {
        addDisplay(d);
    }
}

Display* DisplayManager::getDisplayByBitmask(DisplayBitmask displayBitmask) {
    for(Display* display : displays) {
        if(display->getBitmask() & displayBitmask) return display;
    }
    return nullptr;
}

Display* DisplayManager::getDisplayByPosition(u8 position) {
    for(Display* display : displays) {
        if(display->getPosition() == position) return display;
    }
    return nullptr;
}

void DisplayManager::clearDisplays() {
    for(Display* display : displays) {
        display->setBitmap(Bitmaps::Other::empty);
    }
    requestRedraw();
}

void DisplayManager::addGlobalEffect(Effect* effect) {
    globalEffects.push_back(effect);
}

void DisplayManager::addOverlay(Effect* effect, u8 id) {
    overlays.push_back(Overlay{effect, id});
}

void DisplayManager::removeOverlay(u8 id) {
    for(i32 i = overlays.size() - 1; i >= 0; i--) {
        if(overlays[i].id == id) overlays.erase(overlays.begin() + i);
    }
}

void DisplayManager::clearOverlays() {
    for(i32 i = overlays.size() - 1; i >= 0; i--) {
        if(!overlays[i].effect->applyOnRedraw) {
            overlays.erase(overlays.begin() + i);
        }
    }
}

void DisplayManager::clearAllOverlays() {
    overlays.clear();
}

void DisplayManager::clearParticles() {
    for(Display* d : displays) {
        d->particleSystem->clear();
    }
}

void DisplayManager::setKeyframe(Keyframe* k) {
    keyframe = k;
}

void DisplayManager::redraw(Keyframe* keyframe) {
    // Step 1: Copy bitmap to bitmap Buffer
    // draw bitmaps
    for(const BitmapDraw& b : keyframe->bitmaps) {
        getDisplays(b.displayBitmask);
        for(Display* d : displaysToApply) {
            d->setBitmap(b.bitmap);
        }
    }
    // apply effects
    for(Effect* e : keyframe->effects) {
        if(!(e->displayBitmask & Config::Displays::BITMAPBUFFER)) continue;
        getDisplays(e->displayBitmask);
        for(Display* d : displaysToApply) {
            d->applyEffect(d->bitmapBuffer, e);
        }
    }
    // apply overlays
    for(Overlay& o : overlays) {
        if(!(o.effect->displayBitmask & Config::Displays::BITMAPBUFFER)) continue;
        getDisplays(o.effect->displayBitmask);
        for(Display* d : displaysToApply) {
            d->applyOverlay(d->bitmapBuffer, o);
        }
    }
    redraw(keyframe->effects);
}

void DisplayManager::redraw(std::vector<Effect*>& effects) {
    // Step 2: Store previous frame, create copy and apply remaining effects,
    // store effects that should be applied on redraw, render particles
    for(Display* d : displays) {
        d->copy(d->bitmapBuffer, d->hudBuffer);
        d->copy(d->matrixBuffer, d->previousFrame);
    }

    bool isEffectBuffer = &effects == &effectBuffer;

    if(!isEffectBuffer) effectBuffer.clear();

    // apply effects
    for(Effect* e : effects) {
        if(e->displayBitmask & Config::Displays::BITMAPBUFFER) continue;
        getDisplays(e->displayBitmask);
        for(Display* d : displaysToApply) {
            d->applyEffect(d->hudBuffer, e);
        }
        if(!isEffectBuffer && e->applyOnRedraw) {
            effectBuffer.push_back(e);
        }
    }
    for(Display* d : displays) {
        d->renderParticles();
    }
    // apply overlays
    for(Overlay& o : overlays) {
        if(o.effect->displayBitmask & Config::Displays::BITMAPBUFFER) continue;
        getDisplays(o.effect->displayBitmask);
        for(Display* d : displaysToApply) {
            d->applyOverlay(d->hudBuffer, o);
        }
    }

    // Step 3: Copy to final matrix buffer and apply global overlays
    for(Display* d : displays) {
        // Copy to matrix buffer
        d->copy(d->hudBuffer, d->matrixBuffer);
    }
    for(Effect* e : globalEffects) {
        // Apply all effects used by display
        getDisplays(e->displayBitmask);
        for(Display* d : displaysToApply) {
            d->applyEffect(d->matrixBuffer, e);
        }
    }

    clearOverlays();
    redrawRequested = false;
}

void DisplayManager::redraw() {
    redraw(effectBuffer);
}

void DisplayManager::requestRedraw() {
    redrawRequested = true;
}

void DisplayManager::finalize(Display* display) {
    // only draw lines that are updated
    for(u8 i = 0; i < display->getSize(); i++) {
        if(display->matrixBuffer[i] != display->previousFrame[i]) {
            matrix->drawColumn(display->getPosition() + i, display->matrixBuffer[i]);
        }
    }
    hud->drawBitmap(display->hudBuffer, display->getSize(), display->hudPreviewPosition);
}

void DisplayManager::finalize() {
    for(Display* d : displays) {
        finalize(d);
    }
}

void DisplayManager::forceFinalize(Display* display) {
    matrix->drawBitmap(display->matrixBuffer, display->getSize(), display->getPosition());
}

void DisplayManager::forceFinalize() {
    for(Display* d : displays) {
        forceFinalize(d);
    }
}

u8 DisplayManager::getMatrixBrightness() {
    return matrix->getBrightness();
}

void DisplayManager::setMatrixBrightness(u8 value) {
    if(value > 15) return;
    matrix->setBrightness(value);
    if(enabled) forceFinalize();
    TOASTER_LOG("Set matrix brightness to %d\n", value);
}

void DisplayManager::start() {
    enabled = true;
    hud->clearRect(0, 0, 64, 16);
    matrix->enable(true);
    forceFinalize();
}

void DisplayManager::stop() {
    enabled = false;
    for(Display* d : displays) {
        Point hudPosition = d->hudPreviewPosition;
        hud->clearRect(hudPosition.x, hudPosition.y, d->getSize(), 8);
    }
    matrix->clear();
    matrix->enable(false);
}

void DisplayManager::pause() {
    enabled = false;
}

void DisplayManager::reset() {
    start();
}

void DisplayManager::update() {
    if(!enabled) return;
    if(keyframe) {
        // if new keyframe has been set
        for(Display* display : displays) {
            display->particleSystem->update();
        }
        redraw(keyframe);
        keyframe = nullptr;
    } else {
        // if particles have been updated or a redraw is requested
        for(Display* display : displays) {
            if(display->particleSystem->update()) {
                requestRedraw();
            }
        }
        if(redrawRequested) {
            ++frameCount;
            redraw();
        }
    }
    // todo: test if finalize can be moved up to save performance
    finalize();
}