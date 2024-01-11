#pragma once

#include "Utilities/Typedefs.h"
#include "config.h"
#include "Effect.h"
#include "Clear.h"

class Blink : public Effect {
  public:
    u8 amount;

    /**
     * Draws blank lines on the top and bottom of the buffer according to amount.
     * @param displayBitmask Display bitmask
     * @param amount Number of lines to draw
     */
    Blink(DisplayBitmask displayBitmask, u8 amount)
        : Effect(displayBitmask)
        , amount(amount) {
    }

    Blink(u8 amount)
        : Blink(Config::Displays::EYES, amount) {
    }

    static void blink(u8* buffer, size_t size, u8 amount) {
        if(amount > size / 2) {
            Clear::clear(buffer, size);
            return;
        }
        for(u8 i = 0; i <= size; i += 8) {
            for(u8 j = 0; j < amount; j++) {
                buffer[i + j] = 0;
                buffer[i + 7 - j] = 0;
            }
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        blink(buffer, display->getSize(), amount);
    }
};