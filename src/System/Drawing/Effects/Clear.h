#pragma once

#include "Typedefs.h"
#include "config.h"
#include "Effect.h"

class Clear : public Effect {
  public:
    /**
     * Clears the buffer.
     * @param displayBitmask Display bitmask
     */
    Clear(DisplayBitmask displayBitmask = Config::Displays::ALL)
        : Effect(displayBitmask) {
    }

    static void clear(u8* buffer, size_t size) {
        std::fill(buffer, buffer + size, 0);
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        clear(buffer, display->getSize());
    }
};