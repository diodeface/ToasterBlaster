#pragma once

#include "Utilities/Typedefs.h"
#include "Effect.h"

class DrawBitmap : public Effect {
  public:
    const u8* bitmap;
    size_t bitmapSize;

    /**
     * Draws a bitmap.
     * @param displayBitmask Display bitmask
     * @param bitmap Pointer to bitmap to draw
     * @param bitmapSize Bitmap size (Amount of rows)
     */
    DrawBitmap(DisplayBitmask displayBitmask, const u8* bitmap, size_t bitmapSize)
        : Effect(displayBitmask)
        , bitmap(bitmap)
        , bitmapSize(bitmapSize) {
    }

    static void drawBitmap(u8* buffer, size_t size, const u8* bitmap, size_t bitmapSize) {
        if(size != bitmapSize) return;
        for(i32 i = 0; i < size; i++) buffer[i] = bitmap[i];
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        drawBitmap(buffer, display->getSize(), bitmap, bitmapSize);
    }
};