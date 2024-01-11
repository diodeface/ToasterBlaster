#pragma once

#include "Utilities/Typedefs.h"
#include "Utilities/Util.h"
#include "Effect.h"

class SetPixel : public Effect {
  public:
    u8 x, y;
    DrawMode drawMode;

    /**
     * Set a pixel to a state.
     * @param displayBitmask Display bitmask
     * @param x X Position
     * @param y Y position
     * @param drawMode Drawing mode (DRAW_MODE_0, DRAW_MODE_1, DRAW_MODE_XOR)
     */
    SetPixel(DisplayBitmask displayBitmask, u8 x, u8 y, DrawMode drawMode)
        : Effect(displayBitmask)
        , x(x)
        , y(y)
        , drawMode(drawMode) {
    }

    static void setPixel(u8* buffer, size_t size, u8 x, u8 y, DrawMode drawMode = DRAW_MODE_1) {
        if(x >= size || y >= 8) return;
        Point p = pointToMatrix(x, y);
        // if (p.y > size) return;
        switch(drawMode) {
            case DRAW_MODE_0:
                buffer[p.y] &= ~(1 << p.x);
                break;
            case DRAW_MODE_1:
                buffer[p.y] |= (1 << p.x);
                break;
            case DRAW_MODE_XOR:
                buffer[p.y] ^= (1 << p.x);
                break;
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        setPixel(buffer, display->getSize(), x, y, drawMode);
    }
};