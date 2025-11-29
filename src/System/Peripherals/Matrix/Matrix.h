#pragma once

#include "Typedefs.h"

/**
 * Base class for outer LED matrix.
 */
class Matrix {
  protected:
    u8 brightness;

  public:
    Matrix(u8 brightness = 0) : brightness(brightness) {
    }

    virtual void init() {
    }

    virtual void drawColumn(u8 col, u8 value) {
    }

    virtual void drawBitmap(const u8* bitmap, size_t size, u8 position) {
    }
    
    virtual void setBrightness(u8 brightness) {
    }

    virtual void clear() {
    }

    virtual void enable(bool enable) {
    }

    u8 getBrightness() {
        return brightness;
    }
};