#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "Typedefs.h"
#include "System/Components/ToasterComponent.h"
#include "System/Utilities/Globals.h"
#include "System/Utilities/Util.h"

#include "LEDStrip.h"

/**
 * LED Strip CD animation
 */

class LEDStrip_CD : public LEDStrip {
  public:
    LEDStrip_CD(bool enabled = true) 
    : LEDStrip(enabled) {
        period = 16 MILLIS;
    }

    void animate() override {
        u8 o = ++t * 4;
        CHSV rainbow = CHSV(o, 255, 128 * brightness);

        for (size_t i = 0; i < NUM_LEDS; i++) {
            u8 v = sin8(o - i * 40) * brightness;
            leds[i] = CRGB(v, v, v) + rainbow;
        }

        FastLED.show();
    }
};