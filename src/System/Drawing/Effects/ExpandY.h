#pragma once

#include "Typedefs.h"
#include "config.h"
#include "Effect.h"
#include "SetColumn.h"

class ExpandY : public Effect {
  public:
    u8 amount, column;

    /**
     * Expands all pixels in the selected column by an amount.
     * @param displayBitmask Display bitmask
     * @param column Column to expand
     * @param amount Expand amount
     */
    ExpandY(DisplayBitmask displayBitmask, u8 column, u8 amount)
        : Effect(displayBitmask)
        , column(column)
        , amount(amount) {
    }

    static void expandY(u8* buffer, size_t size, u8 column, u8 amount) {
        if(amount == 0) return;
        u8 temp = getColumn(buffer, size, column);
        u8 ret = temp;

        u8 aboveamt = amount / 2;
        u8 belowamt = amount - aboveamt;

        for(u8 i = 0; i < Config::DISPLAY_HEIGHT; i++) {
            if(bitRead(temp, i)) {
                // above
                for(u8 j = 1; j <= aboveamt; j++) {
                    if (i - j < 0) break;
                    ret = setBit(ret, i-j);
                }
                // below
                for(u8 j = 1; j <= belowamt; j++) {
                    if (i + j > 7) break;
                    ret = setBit(ret, i+j);
                }
            }
        }

        SetColumn::setColumn(buffer, size, column, ret);
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        expandY(buffer, display->getSize(), column, amount);
    }
};