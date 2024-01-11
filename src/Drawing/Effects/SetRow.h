#pragma once

#include "Utilities/Typedefs.h"
#include "Effect.h"

class SetRow : public Effect {
  public:
    u8 row, value;

    /**
     * Draws an 8 pixel row according to the bits in an integer.
     * @param displayBitmask Display bitmask
     * @param row Row (y position) where the row is drawn
     * @param value Integer used to determine if a given x position should be on or off
     */
    SetRow(DisplayBitmask displayBitmask, u8 row, u8 value)
        : Effect(displayBitmask)
        , row(row)
        , value(value) {
    }

    static void setRow(u8* buffer, size_t size, u8 row, u8 value) {
        if(row > size) return;
        buffer[row] = value;
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        setRow(buffer, display->getSize(), row, value);
    }
};