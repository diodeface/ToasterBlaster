#pragma once

// forgive me lord, for i am about to break DRY principle

#include <Arduino.h>
#include <vector>
#include "Utilities/Typedefs.h"
#include "Components/ToasterComponent.h"
#include "Drawing/DisplayManager.h"
#include "Overlay.h"

#define CURRENT sequence->keyframes[index]

/**
 * Overlay sequence player
 *
 * Pretty much the same functionality as SequencePlayer,
 * it applies an OverlaySequence on top of what's currently displayed.
 *
 * You can create multiple instances to have different overlays running on top
 * of each other.
 */
class OverlayPlayer : public ToasterComponent {
  private:
    u8 id;

    Timestamp time, period;
    Timestamp minWait, maxWait, waitTime, waitPeriod;
    i32 index, repeat;
    bool enableRepeat, clearOnFinish;
    DisplayManager* displayManager;
    OverlaySequence* sequence;

    void pushEffects();

    void nextKeyframe();

  protected:
    static u8 globalId;

  public:
    bool transitionFinished;

    OverlayPlayer(
        DisplayManager* displayManager, 
        OverlaySequence* sequence, 
        bool enabled, 
        bool enableRepeat = true, 
        bool clearOnFinish = true
    );

    void reset() override;

    void clearEffects();

    // starts the player with delay in mind
    void start() override;

    // starts the player without waiting
    void forceStart();

    void pause() override;

    void stop() override;

    void setSequence(OverlaySequence* sequence);

    void update() override;

    void setWaitTime();

    void setWaitTime(u32 minWait, u32 maxWait);
};