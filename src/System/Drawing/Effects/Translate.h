#pragma once

#include <algorithm>
#include "config.h"
#include "System/Utilities/Util.h"
#include "Typedefs.h"
#include "Effect.h"
#include "SetPixel.h"

class Translate : public Effect {
  public:
    i8 x, y;

    /**
     * Translates contents of the buffer on x and y axis.
     * @param displayBitmask Display bitmask
     * @param x X amount to translate
     * @param y Y amount to translate
     */
    Translate(DisplayBitmask displayBitmask, i8 x, i8 y)
        : Effect(displayBitmask)
        , x(x)
        , y(y) {
    }

    static void translate(u8* buffer, i8 size, i8 x, i8 y) {
        static i8 displayHeight = (i8)Config::DISPLAY_HEIGHT;
        // return if no change
        if (x == 0 && y == 0) return;

        // return if out of bounds
        if (y >= displayHeight || y <= -displayHeight || x >= size || x <= -size) {
            std::fill(buffer, buffer + size, 0);
            return;
        }

        // shift arrays every multiples of 8 on x axis + y
        u8 ret[size] = {0};
        i8 yShift = (x / displayHeight) * displayHeight + y;

        if (yShift > 0) {
            for (i8 i = 0; i < size; i++) {
                i8 target = yShift + i;
                if(i % displayHeight + y >= displayHeight) continue;
                ret[target] = buffer[i];
            }
        } else if (yShift < 0) {
            for (i8 i = size - 1; i >= 0; i--) {
                i8 target = yShift + i;
                if(i % displayHeight + y < 0) continue;
                ret[target] = buffer[i];
            }
        }
        else {
            std::copy(buffer, buffer + size, ret);
        }

        // bitwise shift for individual rows, bitwise OR with neighbors
        i8 xShift = x % 8;
        if (xShift != 0) {
            i8 axShift = abs(xShift);
            if (xShift > 0) {
                for (i8 i = size - 1; i >= 0; i--) {
                    ret[i] >>= axShift;
                    if (i >= displayHeight) ret[i] |= ret[i-displayHeight] << (displayHeight - axShift);
                }
            }
            else {
                i8 lastColIdx = size - displayHeight;
                for (i8 i = 0; i < size; i++) {
                    ret[i] <<= axShift;
                    if (i < lastColIdx) ret[i] |= ret[i+displayHeight] >> (displayHeight - axShift);
                }
            }
        }

        std::copy(ret, ret + size, buffer);
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        translate(buffer, display->getSize(), x, y);
    }
};