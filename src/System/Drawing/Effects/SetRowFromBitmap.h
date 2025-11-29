#pragma once

#include "Typedefs.h"
#include "Effect.h"

class SetRowFromBitmap : public Effect {
  public:
    u8 row;
    Bitmap* bitmap;

    /**
     * Draws an 8 pixel row from a bitmap.
     * @param displayBitmask Display bitmask
     * @param row Row (y position) where the row is drawn
     * @param bitmap Pointer to bitmap to draw
     */
    SetRowFromBitmap(DisplayBitmask displayBitmask, u8 row, Bitmap* bitmap)
        : Effect(displayBitmask)
        , row(row)
        , bitmap(bitmap) {
    }

    static void setRowFromBitmap(u8* buffer, size_t size, u8 row, Bitmap* bitmap) {
        if(row > size) return;
        buffer[row] = bitmap[row];
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        setRowFromBitmap(buffer, display->getSize(), row, bitmap);
    }
};