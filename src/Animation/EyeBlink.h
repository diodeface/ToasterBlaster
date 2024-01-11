#pragma once

#include "Components/ToasterComponent.h"
#include "Drawing/DisplayManager.h"
#include "Animation/Overlay/OverlaySequence.h"
#include "Animation/Overlay/OverlayPlayer.h"

// todo: this could be used to manually trigger a glitched animation or 
// something similar, consider naming this class something more generic?

/**
 * An improved eye blinking overlay, uses state to allow manual eye closing.
 */
class EyeBlink : public ToasterComponent {
  private:
    enum EyeBlinkState {
        EYEBLINK_DEFAULT,
        EYEBLINK_CLOSING,
        EYEBLINK_CLOSED,
        EYEBLINK_OPENING
    };

    EyeBlinkState state;

    DisplayManager* displayManager;
    OverlayPlayer* overlayPlayer;

    OverlaySequence* currentSequence;
    OverlaySequence *closingSequence, *closedSequence, *openingSequence;

    Timestamp time, blinkPeriod, blinkPeriodMin, blinkPeriodMax;

    bool buttonState;

  public:
    EyeBlink(
        DisplayManager* displayManager,
        OverlaySequence* closingSequence, 
        OverlaySequence* closedSequence, 
        OverlaySequence* openingSequence,
        Timestamp blinkPeriodMin = 5 SECONDS,
        Timestamp blinkPeriodMax = 20 SECONDS,
        bool enabled = true
    ) 
        : displayManager(displayManager)
        , closingSequence(closingSequence)
        , closedSequence(closedSequence)
        , openingSequence(openingSequence)
        , blinkPeriodMin(blinkPeriodMin)
        , blinkPeriodMax(blinkPeriodMax)
        , time(0)
        , buttonState(false)
        , state(EYEBLINK_DEFAULT)
    {
        this->enabled = enabled;
        setBlinkPeriod();
        overlayPlayer = new OverlayPlayer(displayManager, openingSequence, false, false, false);
        TOASTER_LOG("Eye blink initialized.\n");
    }

    void setBlinkPeriod() {
        time = 0;
        if (!enabled) blinkPeriod = UINT32_MAX;     // wow this is hacky
        blinkPeriod = random(blinkPeriodMin, blinkPeriodMax);
        TOASTER_LOG("Eye blink in %dms\n", blinkPeriod / 1000);
    }

    void handleInput(bool input) {
        buttonState = input;
        if(state == EYEBLINK_DEFAULT && input == true) {     
            blinkPeriod = 0;     // manually start close animation if button was pressed
        }
    }

    void stop() override {
        enabled = false;
        setBlinkPeriod();
    }

    void start() override {
        enabled = true;
        setBlinkPeriod();
    }

    void reset() override {
        // uhhhhh idk
        overlayPlayer->clearEffects();
        start();
    }

    // todo: implement stop, start, reset functions that clear the overlay

    void update() override {
        switch(state) {
            case EYEBLINK_DEFAULT:      // wait for time to reach blinkPeriod
                if((time += deltaTime) < blinkPeriod) break;
                state = EYEBLINK_CLOSING;
                overlayPlayer->setSequence(closingSequence);
                break;

            case EYEBLINK_CLOSING:      // wait until closing animation is complete
                if(overlayPlayer->isEnabled()) break;
                state = EYEBLINK_CLOSED;
                overlayPlayer->setSequence(closedSequence);
                break;

            case EYEBLINK_CLOSED:       // wait until buttonState is 0
                if(buttonState == 1) break;
                state = EYEBLINK_OPENING;
                overlayPlayer->setSequence(openingSequence);
                break;

            case EYEBLINK_OPENING:      // wait until opening animation is complete
                if(overlayPlayer->isEnabled()) break;
                state = EYEBLINK_DEFAULT;
                overlayPlayer->clearEffects();
                setBlinkPeriod();
                break;
        }

        overlayPlayer->update();
    }
};