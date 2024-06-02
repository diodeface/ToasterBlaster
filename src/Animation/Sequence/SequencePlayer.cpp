#include "SequencePlayer.h"

void SequencePlayer::nextKeyframe() {
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
            default:
                index = CURRENT.next;
        }

        // if index is out of bounds we stop the animation
        if(index >= sequence->keyframes.size()) {
            endAnimation();
            return;
        }
    }

    for(Effect* e : CURRENT.effects) {
        e->reset();
    }
    repeat = CURRENT.repeat;
}

OverlaySequence* SequencePlayer::getRandomTransition() {
    if(random() % 100 < rareTransitionChance) {
        if (rareTransitions.size() == 0) return nullptr;
        return rareTransitions[random(0, rareTransitions.size())];
    } else {
        if (commonTransitions.size() == 0) return nullptr;
        return commonTransitions[random(0, commonTransitions.size())];
    }
}

SequencePlayer::SequencePlayer(
    DisplayManager* displayManager, 
    TweenManager* tweenManager,
    EyeBlink* eyeBlink,
    Sequence* sequence, 
    u8 rareTransitionChance, 
    bool enabled
)
    : displayManager(displayManager)
    , tweenManager(tweenManager)
    , eyeBlink(eyeBlink)
{
    this->enabled = enabled;
    setSequence(sequence);      // todo: reset virtual function call
    transitionOverlay = new OverlayPlayer(displayManager, &Transitions::blink, false, false);
    setRareTransitionChance(rareTransitionChance);
    TOASTER_LOG(
        "SequencePlayer initialized with sequence %s, state: %s.\n", sequence->name,
        enabled ? "enabled" : "disabled"
    );
}

void SequencePlayer::reset() {
    index = -1;
    enabled = true;
    time = 0;
    waitTime = 0;

    if(sequence) {
        setWait(sequence->minWait, sequence->maxWait);
        nextKeyframe();
    }
}

void SequencePlayer::start() {
    enabled = true;
}

void SequencePlayer::pause() {
    enabled = false;
}

void SequencePlayer::endAnimation() {
    TOASTER_LOG("Sequenceplayer sequence %s end reached\n", sequence->name);
    reset();
    enabled = false;
}

void SequencePlayer::stop() {
    if (!sequence && !nextSequence) return;
    endAnimation();
    transitionOverlay->stop();
    sequence = nullptr;
    nextSequence = nullptr;
}

void SequencePlayer::setSequence(Sequence* sequence) {
    if(!sequence) return;
    this->sequence = sequence;
    nextSequence = nullptr;
    // displayManager->clearParticles();     // todo: do something about particles
    displayManager->hud->drawStatusText(sequence->name);
    reset();
    waitTime = 0;
}

bool SequencePlayer::transitionSequence(Sequence* sequence) {
    // todo: clean up this mess?
    eyeBlink->setBlinkPeriod();
    tweenManager->clearInterruptable();
    displayManager->clearParticles();

    // no transition is set (nullptr)
    if(!sequence->transition) {
        transitionOverlay->stop();
        setSequence(sequence);

    // use a random transition
    } else if(sequence->transition == &Transitions::any) {
        if(OverlaySequence* transition = getRandomTransition()) {
            transitionOverlay->setSequence(transition);
            transitionOverlay->forceStart();
        } else {
            transitionOverlay->stop();
            setSequence(sequence);
        }

    // use provided transition
    } else {
        transitionOverlay->setSequence(sequence->transition);
        transitionOverlay->forceStart();
    }

    nextSequence = sequence;
    displayManager->hud->drawStatusText(sequence->name);
    return true;
}

void SequencePlayer::update() {
    // if a sequence change is due
    if(transitionOverlay->transitionFinished &&
       // sequence != nextSequence &&
       nextSequence) {
        transitionOverlay->transitionFinished = false;
        setSequence(nextSequence);
    }

    // if time to draw next frame has been reached
    if(sequence && enabled && (waitTime += deltaTime) > waitPeriod && (time += deltaTime) > period) {
        time = 0;

        displayManager->setKeyframe(&CURRENT);
        period = CURRENT.duration;

        TOASTER_LOG(
            "SequencePlayer [Sequence: %s, Index: %2d/%2d, period: %5d, Repeat: %2d, Next: %2d]\n", sequence->name,
            index, sequence->keyframes.size() - 1, period, repeat, CURRENT.next
        );

        // when the last keyframe is reached set a random wait time
        if(index == sequence->keyframes.size() - 1) {
            setWait();
            waitTime = 0;
            time = waitPeriod;
        }

        if(repeat-- == 0) nextKeyframe();
    }
    transitionOverlay->update();
}

void SequencePlayer::setWait() {
    waitPeriod = random(minWait, maxWait);
    TOASTER_LOG("SequencePlayer sequence %s will start in %dms.\n", sequence->name, waitPeriod / 1000);
}

void SequencePlayer::setWait(u32 minWait, u32 maxWait) {
    this->minWait = minWait;
    this->maxWait = maxWait;
    setWait();
}

Sequence* SequencePlayer::getSequence() {
    return nextSequence ? nextSequence : sequence;
}

void SequencePlayer::addCommonTransitions(std::initializer_list<Transition*> transitions) {
    commonTransitions.insert(commonTransitions.end(), transitions);
}

void SequencePlayer::addRareTransitions(std::initializer_list<Transition*> transitions) {
    rareTransitions.insert(rareTransitions.end(), transitions);
}

u8 SequencePlayer::getRareTransitionChance() {
    return rareTransitionChance;
}

void SequencePlayer::setRareTransitionChance(u8 chance) {
    if (chance > 100) rareTransitionChance = 100;
    else rareTransitionChance = chance;
    TOASTER_LOG("Set rare transition chance to %d\n", rareTransitionChance);
}

void SequencePlayer::addRareTransitionChance(i8 value) {
    i32 newValue = rareTransitionChance + value;
    if(newValue > 255) setRareTransitionChance(255);
    else if(newValue < 0) setRareTransitionChance(0);
    else setRareTransitionChance(newValue);
}