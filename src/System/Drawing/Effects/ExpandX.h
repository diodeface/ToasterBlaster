#pragma once

#include "Typedefs.h"
#include "config.h"
#include "Effect.h"
#include "SetPixel.h"

class ExpandX : public Effect {
  public:
    u8 iterations;

    /**
     * Expands all pixels on X axis.
     * @param displayBitmask Display bitmask
     * @param iterations Expand amount
     */
    ExpandX(DisplayBitmask displayBitmask, u8 iterations)
        : Effect(displayBitmask)
        , iterations(iterations) {
    }

    static void expandX(u8* buffer, size_t size, u8 iterations) {
        if(iterations == 0) return;
        if(iterations > Config::DISPLAY_HEIGHT) iterations = Config::DISPLAY_HEIGHT;

        for(u8 iter = 0; iter < iterations; iter++) {
            u8 temp[size];
            std::copy(buffer, buffer + size, temp);
            for(i8 i = 0; i < size; i++) {
                for(i8 j = 0; j < Config::DISPLAY_HEIGHT; j++) {
                    if(getPixel(temp, size, i, j)) {
                        SetPixel::setPixel(buffer, size, i + 1, j);
                        SetPixel::setPixel(buffer, size, i - 1, j);
                    }
                }
            }
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        expandX(buffer, display->getSize(), iterations);
    }
};