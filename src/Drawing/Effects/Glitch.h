#pragma once

#include "Utilities/Typedefs.h"
#include "Effect.h"

class Glitch : public Effect {
  public:
    u8 intensity, maxShift;

    /**
     * Randomly shifts the rows in a buffer.
     * @param displayBitmask Display bitmask
     * @param intensity Chance for a row to shift
     * @param maxShift Maximum shift amount
     */
    Glitch(DisplayBitmask displayBitmask, u8 intensity, u8 maxShift)
        : Effect(displayBitmask)
        , intensity(intensity)
        , maxShift(maxShift) {
    }

    Glitch(u8 intensity, u8 maxShift)
        : Glitch(Config::Displays::ALL, intensity, maxShift) {
    }

    static void glitch(u8* buffer, size_t size, u8 intensity, u8 maxShift) {
        if(maxShift == 0 || intensity == 0) return;
        if(maxShift > 7) maxShift = 7;
        for(u8 i = 0; i < size; i++) {
            if(random() % 100 < intensity) {
                u8 shiftamt = random() % maxShift;
                buffer[i] = random() % 2 == 0 ? buffer[i] << shiftamt : buffer[i] >> shiftamt;
            }
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        glitch(buffer, display->getSize(), intensity, maxShift);
    }
};