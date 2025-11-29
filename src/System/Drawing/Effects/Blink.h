#pragma once

#include "Typedefs.h"
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
        if(amount > size) {
            Clear::clear(buffer, size);
            return;
        }
        u8 split1 = amount / 2;
        u8 split2 = 8 - (amount - split1);
        for(u8 i = 0; i <= size - 8; i += 8) {
            for(u8 j = 0; j < split1; j++) {
                buffer[i + j] = 0;
            }
            for(u8 j = split2; j < 8; j++) {
                buffer[i + j] = 0;
            }
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        blink(buffer, display->getSize(), amount);
    }
};