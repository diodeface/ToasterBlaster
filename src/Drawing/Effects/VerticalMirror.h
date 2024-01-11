#pragma once

#include "Utilities/Typedefs.h"
#include "Effect.h"

class VerticalMirror : public Effect {
  public:
    /**
     * Vertically mirrors the buffer.
     * @param displayBitmask Display bitmask
     */
    VerticalMirror(DisplayBitmask displayBitmask)
        : Effect(displayBitmask){};

    static void verticalMirror(u8* buffer, size_t size) {
        for(u8 i = 0; i < size; i += 8) {
            for(u8 j = 0; j < 4; j++) {
                u8 temp = buffer[i + j];
                buffer[i + j] = buffer[i + 7 - j];
                buffer[i + 7 - j] = temp;
            }
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        verticalMirror(buffer, display->getSize());
    }
};