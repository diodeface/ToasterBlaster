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

    void initialize() {
        u8 step = 255 / (NUM_LEDS / 2);
        for(u8 i = 0; i < NUM_LEDS / 2; i++) {
            values[i] = values[NUM_LEDS - i - 1] = 255 - i * step;
            leds[i] = leds[NUM_LEDS - i - 1] = CRGB(values[i], 0, 0);
        }
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

    LEDStrip(bool enabled = true) {
        this->enabled = enabled;

        leds = new CRGB[NUM_LEDS];
        values = new u8[NUM_LEDS];

        time = 0;
        period = 30000;

        FastLED.addLeds<WS2812, 15, GRB>(leds, NUM_LEDS);

        if(enabled) {
            initialize();
        } else {
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
        initialize();
    }

    void stop() override {
        enabled = false;
        clearLEDs();
    }

    void pause() override {
        enabled = false;
    }

    void reset() override {
        start();
    }

    void update() override {
        if(!enabled) return;
        if((time += deltaTime) < period) return;
        time = 0;

        for(u8 i = 0; i < NUM_LEDS; i++) {
            values[i] -= speed;
            if(values[i] <= 63) {
                leds[i].r = values[i] * 4;
            } else if(values[i] <= 127) {
                leds[i].r = 255 - (values[i] - 64) * 4;
            } else {
                leds[i].r = 0;
            }
        }

        if(speed > 1) --speed;
        FastLED.show();
    }
};