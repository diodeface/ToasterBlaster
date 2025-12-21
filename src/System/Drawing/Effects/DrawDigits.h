#pragma once

#include "Typedefs.h"
#include "config.h"
#include "Effect.h"
#include "SetColumn.h"
#include "Assets/Bitmaps/Standard.h"

class DrawDigits : public Effect {
  public:
    u32 value;

    /**
     * Draws digits of a positive integer.
     * @param displayBitmask Display bitmask
     * @param value value to draw
     */
    DrawDigits(DisplayBitmask displayBitmask, u32 value)
        : Effect(displayBitmask)
        , value(value) {
    }

    static void drawDigit(u8* buffer, size_t size, u8 digit, u8 x = 0) {
        for (u8 i = 0; i < 3; i++) {
            SetColumn::setColumn(buffer, size, x+i, Bitmaps::System::digits[digit][i]);
        }
        SetColumn::setColumn(buffer, size, x+3, 0);
    }

    static void drawDigits(u8* buffer, size_t size, u32 value) {
        u8 x = 0;
        char temp[11];
        sprintf(temp, "%d", value);
        char* digit = temp;
        while (*digit != '\0') {
            drawDigit(buffer, size, *digit & 0xf, x);
            x += 4;
            if (x > size) break;
            ++digit;
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        drawDigits(buffer, display->getSize(), value);
    }
};