#pragma once

#include "Utilities/Util.h"
#include "Animation/Bitmaps.h"

/**
 * Specifies on which displays should a bitmap be drawn.
 * @param displayBitmask Display Bitmask
 * @param bitmap Pointer to raw bitmap
 */
struct BitmapDraw {
    DisplayBitmask displayBitmask;
    const Bitmap* bitmap;
};