#pragma once

#include "Utilities/Typedefs.h"
#include "Effect.h"
#include "Clear.h"
#include "SetColumn.h"

class Rotate90 : public Effect {
  public:
    bool direction;

    /**
     * Rotate the buffer 90 degrees in either direction.
     * Only works on 8 pixel tall displays.
     * @param displayBitmask Display bitmask
     * @param direction True for clockwise, False for counter-clockwise
     */
    Rotate90(DisplayBitmask displayBitmask = Config::Displays::ALL, bool direction = false)
        : Effect(displayBitmask)
        , direction(direction) {
    }

    static void rotate90(u8* buffer, size_t size, bool direction) {
        u8 temp[8];
        if((size / 8) % 2) {
            for(u8 i = 0; i < 8; i++) {
                temp[i] = buffer[i];
                buffer[i] = 0;
            }
        } else {
            u8 pos = (size == 16) ? 0 : 8;
            for(u8 i = 0; i < 8; i++) {
                temp[i] = ((buffer[i + pos] & 0b1111) << 4) | ((buffer[i + 8 + pos] & 0b11110000) >> 4);
            }
            Clear::clear(buffer, size);
        }

        u8 offset = (size > 8) ? size / 2 - 4 : 0;
        if(direction)
            for(u8 i = 0; i < 8; i++) SetColumn::setColumn(buffer, size, i + offset, mirrorInt(temp[i]));
        else
            for(u8 i = 0; i < 8; i++) SetColumn::setColumn(buffer, size, 7 - i + offset, temp[i]);
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        rotate90(buffer, display->getSize(), direction);
    }
};