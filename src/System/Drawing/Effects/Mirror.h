#pragma once

#include "Typedefs.h"
#include "Effect.h"

class Mirror : public Effect {
  public:
    /**
     * Mirrors the buffer. Only works with 8 pixel tall displays.
     * @param displayBitmask Display bitmask
     */
    Mirror(DisplayBitmask displayBitmask = Config::Displays::ALL)
        : Effect(displayBitmask) {
    }

    static void mirror(u8* buffer, size_t size) {
        if(size == 8) {
            for(u8 i = 0; i < size; i++) {
                buffer[i] = mirrorInt(buffer[i]);
            }
            return;
        }

        for(u8 i = 0; i < size / 16; i++) {
            for(u8 j = 0; j < 8; j++) {
                u8 temp = mirrorInt(buffer[i * 8 + j]);
                buffer[i * 8 + j] = mirrorInt(buffer[size - (i + 1) * 8 + j]);
                buffer[size - (i + 1) * 8 + j] = temp;
            }
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        mirror(buffer, display->getSize());
    }
};