#pragma once

#include "Typedefs.h"
#include "config.h"
#include "Effect.h"
#include "Translate.h"

class Earthquake : public Effect {
  public:
    u8 amplitudeX, amplitudeY;

    /**
     * Offsets the bitmap randomly according to amplitude.
     * @param displayBitmask Display bitmask
     * @param amplitudeX X amplitude in pixels
     * @param amplitudeX Y amplitude in pixels
     */
    Earthquake(DisplayBitmask displayBitmask = Config::Displays::ALL, u8 amplitudeX = 4, u8 amplitudeY = 4)
        : Effect(displayBitmask)
        , amplitudeX(amplitudeX)
        , amplitudeY(amplitudeY) {
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        i8 offsetX = random(-amplitudeX, amplitudeX);
        i8 offsetY = random(-amplitudeX, amplitudeX);
        Translate::translate(display->hudBuffer, display->getSize(), offsetX, offsetY);
    }
};