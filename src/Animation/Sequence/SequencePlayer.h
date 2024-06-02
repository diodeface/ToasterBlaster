#pragma once

#include "Arduino.h"
#include <vector>
#include "Components/ToasterComponent.h"
#include "Animation/Overlay/OverlayPlayer.h"
#include "Drawing/DisplayManager.h"
#include "Animation/Interpolation/TweenManager.h"
#include "Animation/Sequence/Sequences.h"
#include "Animation/EyeBlink.h"

#define CURRENT sequence->keyframes[index]

/**
 * Sequence player
 *
 * Handles animation timing, keyframe index and sending current keyframe data to
 * DisplayManager in order to display it.
 */
class SequencePlayer : public ToasterComponent {
  private:
    //f32 animationSpeed = 1.0f;  // todo: animation speed
    Timestamp time, period;
    Timestamp waitTime, waitPeriod, minWait, maxWait;
    i32 index, repeat;
    DisplayManager* displayManager;
    TweenManager* tweenManager;
    Sequence *sequence, *nextSequence;
    OverlayPlayer* transitionOverlay;
    std::vector<Transition*> commonTransitions;
    std::vector<Transition*> rareTransitions;
    u8 rareTransitionChance;
    EyeBlink* eyeBlink; // todo: before and after transition callbacks, this is kinda jank

    /**
     * Go to the keyframe defined by the "next" value in the current keyframe.
     *
     * An index that's 0 or greater jumps to that particular keyframe in the sequence.
     * An index that is out of bounds stops the animation.
     * An index of -1 goes to the next keyframe.
     * An index of -2 repeats the current keyframe indefinitely.
     */
    void nextKeyframe();

    OverlaySequence* getRandomTransition();

  public:
    /**
     * Construct a SequencePlayer.
     * @param displayManager DisplayManager instance.
     * @param tweenManager TweenManager instance.
     * @param sequence Initial sequence.
     * @param enabled Whether or not to play the animation after constructing.
     */
    SequencePlayer(
        DisplayManager* displayManager, 
        TweenManager* tweenManager, 
        EyeBlink* eyeBlink,
        Sequence* sequence, 
        u8 rareTransitionChance = 10, 
        bool enabled = true
    );

    void reset() override;

    void start() override;

    void pause() override;

    void endAnimation();

    void stop() override;

    /**
     * Set a sequence to be played. The sequence will start playing immediately.
     * @param sequence Sequence to play.
     */
    void setSequence(Sequence* sequence);

    /**
     * Start the transition defined in the passed sequence.
     * After the transition is finished, switch to the passed sequence.
     * @param sequence Sequence to transition to.
     */
    bool transitionSequence(Sequence* sequence);

    void update() override;

    void setWait();

    void setWait(u32 minWait, u32 maxWait);

    /*
    void resetSpeed() {
        animationSpeed = 1.0f;
    }

    void addSpeed(f32 value) {
        animationSpeed += value;
    }
    */

    Sequence* getSequence();

    void addCommonTransitions(std::initializer_list<Transition*> transitions);

    void addRareTransitions(std::initializer_list<Transition*> transitions);

    u8 getRareTransitionChance();

    void setRareTransitionChance(u8 chance);

    void addRareTransitionChance(i8 value);
};