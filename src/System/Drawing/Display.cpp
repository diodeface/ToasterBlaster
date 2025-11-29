#include "Display.h"

// todo: dependency nonsense
#include "System/Drawing/Effects/Effect.h"
#include "System/Drawing/Effects/SetPixel.h"
#include "System/Animation/Overlay/Overlay.h"

Display::Display(
    u8 position, u8 size, DisplayBitmask displayBitmask, Point hudPreviewPosition, TweenManager* tweenManager
)
    : position(position)
    , size(size)
    , displayBitmask(displayBitmask)
    , hudPreviewPosition(hudPreviewPosition)
    , tweenManager(tweenManager) {
    bitmapBuffer = new u8[size]();
    previousFrame = new u8[size]();
    hudBuffer = new u8[size]();
    matrixBuffer = new u8[size]();
    // todo: lots of particles are a memory hog, change this when porting to platforms with less memory
    particleSystem = new ParticleSystem(size * 8 + 32);
}

Display::~Display() {
    delete[] previousFrame;
    delete[] bitmapBuffer;
    delete[] hudBuffer;
    delete[] matrixBuffer;
}

void Display::renderParticles() {
    for(Particle p : particleSystem->getParticles()) {
        SetPixel::setPixel(hudBuffer, size, (u8)fpm::nearbyint(p.x), (u8)fpm::nearbyint(p.y), p.drawMode);
    }
}

void Display::copy(const u8* from, u8* to) {
    std::copy(from, from + size, to);
}

bool Display::compare(const u8* buf1, const u8* buf2) {
    for(size_t i = 0; i < size; i++) {
        if(buf1[i] != buf2[i]) return false;
    }
    return true;
}

void Display::setBitmap(const Bitmap* bitmap) {
    std::copy(bitmap, bitmap + size, bitmapBuffer);
}

void Display::applyEffect(u8* buffer, Effect* effect) {
    effect->apply(buffer, this);
}

void Display::applyOverlay(u8* buffer, Overlay& overlay) {
    overlay.effect->apply(buffer, this);
}

u8 Display::getPosition() {
    return position;
}

u8 Display::getSize() {
    return size;
}

DisplayBitmask Display::getBitmask() {
    return displayBitmask;
}
