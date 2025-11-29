#pragma once

#include "Typedefs.h"
#include "Effect.h"
#include "SetPixel.h"

class SetColumn : public Effect {
  public:
    u8 col, value;

    /**
     * Draw a 8 pixel column according to the bits in an integer.
     * @param displayBitmask Display bitmask
     * @param col Column (x position) where the column is drawn
     * @param value Integer used to determine if a given y position should be on or off
     */
    SetColumn(DisplayBitmask displayBitmask, u8 col, u8 value)
        : Effect(displayBitmask)
        , col(col)
        , value(value) {
    }

    static void setColumn(u8* buffer, size_t size, u8 col, u8 value) {
        for(u8 i = 0; i < 8; i++) SetPixel::setPixel(buffer, size, col, i, (DrawMode)bitRead(value, 7 - i));
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        setColumn(buffer, display->getSize(), col, value);
    }
};