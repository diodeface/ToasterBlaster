#pragma once

#include "Typedefs.h"
#include "Effect.h"

class TVStatic : public Effect {
  public:
    u8 intensity;

    /**
     * Randomly toggles pixels according to intensity value.
     * @param displayBitmask Display bitmask
     * @param intensity Chance (0 - 100 percent) for a pixel to toggle its state
     */
    TVStatic(DisplayBitmask displayBitmask, u8 intensity)
        : Effect(displayBitmask)
        , intensity(intensity) {
    }

    TVStatic(u8 intensity)
        : TVStatic(Config::Displays::ALL, intensity) {
    }

    static void tvStatic(u8* buffer, size_t size, u8 intensity) {
        for(u8 i = 0; i < size; i++) {
            for(u8 j = 0; j < 8; j++) {
                if(random() % 100 < intensity) {
                    buffer[i] ^= (1 << j);
                }
            }
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        tvStatic(buffer, display->getSize(), intensity);
    }
};