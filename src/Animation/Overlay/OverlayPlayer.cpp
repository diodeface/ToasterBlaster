#include "OverlayPlayer.h"

// initial id for OverlayPlayers
u8 OverlayPlayer::globalId = 32;

void OverlayPlayer::pushEffects() {
    for(Effect* effect : CURRENT.effects) {
        displayManager->addOverlay(effect, id);
    }
    displayManager->requestRedraw();
}

void OverlayPlayer::nextKeyframe() {
    // set next keyframe
    if(index < 0) {
        index = 0;
        period = 0;
    } else {
        switch(CURRENT.next) {
            case -1:
                index += 1;
                break;
            case -2:
                break;
            case -3:
                stop();
                return;
            default:
                index = CURRENT.next;
        }

        if(index == sequence->transitionIndex) {
            transitionFinished = true;
        }

        // if index is out of bounds we stop the animation
        if(index >= sequence->keyframes.size()) {
            stop();
            return;
        }
    }
    for(Effect* e : CURRENT.effects) {
        e->reset();
    }
    repeat = CURRENT.repeat;
}

OverlayPlayer::OverlayPlayer(
    DisplayManager* displayManager, 
    OverlaySequence* sequence, 
    bool enabled, 
    bool enableRepeat, 
    bool clearOnFinish
) {
    id = globalId++;
    this->displayManager = displayManager;
    setSequence(sequence);          // todo: virtual function call
    this->enabled = enabled;
    this->enableRepeat = enableRepeat;
    this->clearOnFinish = clearOnFinish;
    transitionFinished = false;
}

void OverlayPlayer::reset() {
    index = -1;
    enabled = true;
    transitionFinished = false;
    time = 0;
    waitTime = 0;

    setWaitTime(sequence->minWait, sequence->maxWait);
    nextKeyframe();
    if (clearOnFinish) {
        clearEffects();
        displayManager->requestRedraw();
    }
}

void OverlayPlayer::clearEffects() {
    displayManager->removeOverlay(id);
}

// starts the player with delay in mind
void OverlayPlayer::start() {
    enabled = true;
    if(sequence->transitionIndex == 0) transitionFinished = true;
}

// starts the player without waiting
void OverlayPlayer::forceStart() {
    start();
    period = 0;
    waitPeriod = 0;
}

void OverlayPlayer::pause() {
    enabled = false;
}

void OverlayPlayer::stop() {
    reset();
    enabled = false;
}

void OverlayPlayer::setSequence(OverlaySequence* sequence) {
    if(clearOnFinish)clearEffects();
    this->sequence = sequence;
    reset();
}

void OverlayPlayer::update() {
    if(!enabled) return;
    if((waitTime += deltaTime) < waitPeriod) return;
    if((time += deltaTime) < period) return;
    time = 0;

    clearEffects();
    pushEffects();

    period = CURRENT.duration;
    TOASTER_LOG(
        "OverlayPlayer [id: %d, Index: %d/%d, Period: %d, Repeat: %d, Next: %d]\n", id, index,
        sequence->keyframes.size() - 1, period, repeat, CURRENT.next
    );

    // when the last keyframe is reached set a random wait time or stop
    if(index == sequence->keyframes.size() - 1) {
        if(enableRepeat) {
            setWaitTime();
            waitTime = 0;
            time = 0;
        } else {
            stop();
        }
    }

    if(repeat-- == 0) nextKeyframe();
}

void OverlayPlayer::setWaitTime() {
    waitPeriod = random(minWait, maxWait);
    TOASTER_LOG("OverlayPlayer %d will start in %dms.\n", id, waitPeriod / 1000);
}

void OverlayPlayer::setWaitTime(u32 minWait, u32 maxWait) {
    this->minWait = minWait;
    this->maxWait = maxWait;
    setWaitTime();
}