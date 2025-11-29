#pragma once

#include "Typedefs.h"
#include "Effect.h"
#include "ExpandY.h"

class MouthEffect : public Effect {
  public:
    u8* values;
    bool direction;

    /**
     * Expands all columns in a display according to values in an array.
     * @param displayBitmask Display bitmask
     * @param values array of values to use, must be the same length as display size!
     * @param direction direction in which to expand the columns
     */
    MouthEffect(DisplayBitmask displayBitmask, u8* values, bool direction = false)
        : Effect(displayBitmask)
        , values(values)
        , direction(direction) {
    }

    void apply(u8* buffer, Display* display) override {
        size_t size = display->getSize();
        Effect::apply(buffer, display);
        if(direction) {
            for(u8 i = 0; i < size; i++) {
                ExpandY::expandY(buffer, size, i, values[i]);
            }
        } else {
            for(u8 i = 0; i < size; i++) {
                ExpandY::expandY(buffer, size, size - i - 1, values[i]);
            }
        }
    }
};