#pragma once

#include "Utilities/Typedefs.h"
#include <Arduino.h>
#include <FastLED.h>
#include "Components/ToasterComponent.h"

/**
 * LED Strip animation code (work in progress)
 */

class LEDStrip : public ToasterComponent {
  private:
    const u8 LEDSTRIP_PIN = 15;
    const u8 NUM_LEDS = 24;
    u8* values;
    CRGB* leds;
    Timestamp time, period;
    enum LEDStripState {
        LEDSTRIP_DEFAULT,
        LEDSTRIP_STOPPED,
        LEDSTRIP_FADEIN,
        LEDSTRIP_FADEOUT,
        LEDSTRIP_FADEOUT_STOP,
        LEDSTRIP_BREATHE
    } state;

    void initialize() {
        u8 step = 255 / (NUM_LEDS / 2);
        for(u8 i = 0; i < NUM_LEDS / 2; i++) {
            values[i] = values[NUM_LEDS - i - 1] = 255 - i * step;
            leds[i] = leds[NUM_LEDS - i - 1] = CRGB(0, 0, 0);
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
    i8 speed = 1;
    f32 brightness = 0.0f;

    LEDStrip(bool enabled = true) {
        this->enabled = enabled;

        leds = new CRGB[NUM_LEDS];
        values = new u8[NUM_LEDS];

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
        delete[] values;
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

        for(u8 i = 0; i < NUM_LEDS; i++) {
            values[i] -= speed;
            if(values[i] <= 63) {
                u8 val = (values[i] - 64) * 4;
                val *= brightness;
                leds[i] = CRGB(255*brightness, val, val);
            } else if(values[i] <= 127) {
                u8 val = 255 - (values[i] * 4);
                val *= brightness;
                leds[i] = CRGB(255*brightness, val, val);
            } else {
                leds[i] = CRGB(255*brightness, 0, 0);
            }
        }

        //if(speed > 1) --speed;
        FastLED.show();
    }
};