#pragma once

#include "Typedefs.h"
#include "config.h"
#include "Effect.h"
#include "SetPixel.h"

class Expand : public Effect {
  public:
    u8 iterations;

    /**
     * Expands all pixels in 4 directions.
     * @param displayBitmask Display bitmask
     * @param iterations Expand amount
     */
    Expand(DisplayBitmask displayBitmask, u8 iterations)
        : Effect(displayBitmask)
        , iterations(iterations) {
    }

    static void expand(u8* buffer, size_t size, u8 iterations) {
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
                        SetPixel::setPixel(buffer, size, i, j + 1);
                        SetPixel::setPixel(buffer, size, i, j - 1);
                    }
                }
            }
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        expand(buffer, display->getSize(), iterations);
    }
};