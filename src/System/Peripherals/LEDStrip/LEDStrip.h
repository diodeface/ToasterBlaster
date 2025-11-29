#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "Typedefs.h"
#include "System/Utilities/Util.h"
#include "System/Components/ToasterComponent.h"

/**
 * LED Strip animation code (work in progress)
 */

class LEDStrip : public ToasterComponent {
  private:
    const u8 LEDSTRIP_PIN = 15;
  protected:
    const u8 NUM_LEDS = 24;
    CRGB* leds;

    Timestamp time, period;
    u8 t;

    enum LEDStripState {
        LEDSTRIP_DEFAULT,
        LEDSTRIP_STOPPED,
        LEDSTRIP_FADEIN,
        LEDSTRIP_FADEOUT,
        LEDSTRIP_FADEOUT_STOP,
        LEDSTRIP_BREATHE
    } state;

    virtual void initialize() {
        for(u8 i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(0, 0, 0);
        }
        brightness = 0;
        state = LEDSTRIP_FADEIN;
        FastLED.show();
    }

    void clearLEDs() {
        for(u8 i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(0, 0, 0);
        }
        FastLED.show();
    }

  public:
    f32 brightness = 0.0f;

    LEDStrip(bool enabled = true) 
    : period(16)
    , t(0) {
        this->enabled = enabled;

        leds = new CRGB[NUM_LEDS];

        time = 0;
        period = 0;

        FastLED.addLeds<WS2812, 15, GRB>(leds, NUM_LEDS);

        if(enabled) {
            initialize();
        } else {
            state = LEDSTRIP_STOPPED;
            clearLEDs();
        }
        TOASTER_LOG("LED Strip initialized.\n");
    }

    ~LEDStrip() {
        delete[] leds;
    }

    void start() override {
        enabled = true;
        state = LEDSTRIP_FADEIN;
    }

    void stop() override {
        state = LEDStripState::LEDSTRIP_FADEOUT_STOP;
    }

    void pause() override {
        stop();
    }

    void reset() override {
        start();
    }

    bool isEnabled() override {
        return state != LEDSTRIP_STOPPED && state != LEDSTRIP_FADEOUT_STOP;
    }

    virtual void animate() {
        ++t;

        for (size_t i = 0; i < NUM_LEDS; i++) {
            u8 v = sin8(t + i * 10);
            leds[i] = CHSV(0, v, 255 * brightness);
        }

        FastLED.show();
    }

    void update() override {
        if(!enabled) return;
        if((time += deltaTime) < period) return;
        time = 0;

        switch(state) {
            case LEDSTRIP_FADEIN:
                brightness += 0.01f;
                if (brightness > 1.0f) {
                    brightness = 1.0f;
                    state = LEDSTRIP_DEFAULT;
                    TOASTER_LOG("LED Strip fade in finished\n");
                }
                break;
            case LEDSTRIP_FADEOUT_STOP:
                if (brightness <= 0) {
                    state = LEDSTRIP_STOPPED;
                    enabled = false;
                    clearLEDs();
                    TOASTER_LOG("LED Strip stopped\n");
                    return;
                }
            case LEDSTRIP_FADEOUT:
                brightness -= 0.025f;
                if (brightness < 0) {
                    brightness = 0;
                    if(state == LEDSTRIP_FADEOUT) state = LEDSTRIP_DEFAULT;
                    TOASTER_LOG("LED Strip fade out finished\n");
                } 
                break;
            case LEDSTRIP_BREATHE:
                // todo: breathing animation?
                break;
        }

        animate();
    }
};