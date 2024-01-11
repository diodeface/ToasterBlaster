#pragma once

#include <algorithm>
#include "Utilities/Util.h"
#include "Utilities/Typedefs.h"
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

    // todo: This could be faster if rewritten with bitwise operators and array shifts...
    static void translate(u8* buffer, size_t size, i8 x, i8 y) {
        if(x == 0 && y == 0) return;
        u8 ret[size] = {0};
        for(size_t i = 0; i < size; i++) {
            for(size_t j = 0; j < 8; j++) {
                i8 newX = i + x, newY = j + y;
                if(newX > size - 1 || newY > 7) break;
                if(newX < 0 || newY < 0) continue;
                SetPixel::setPixel(ret, size, newX, newY, getPixel(buffer, size, i, j));
            }
        }
        std::copy(ret, ret + size, buffer);
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        translate(buffer, display->getSize(), x, y);
    }
};