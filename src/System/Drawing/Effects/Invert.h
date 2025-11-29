#pragma once

#include "Typedefs.h"
#include "Effect.h"

class Invert : public Effect {
  public:
    /**
     * Inverts all bits in a buffer.
     * @param displayBitmask Display bitmask
     */
    Invert(DisplayBitmask displayBitmask = Config::Displays::ALL)
        : Effect(displayBitmask) {
    }

    static void invert(u8* buffer, size_t size) {
        for(u8 i = 0; i < size; i++) {
            buffer[i] = ~buffer[i];
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        invert(buffer, display->getSize());
    }
};