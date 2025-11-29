#pragma once

#include "config.h"
#include "Typedefs.h"
#include "Effect.h"
#include "SetPixel.h"

class Rectangle : public Effect {
  public:
    u8 x, y, w, h;
    DrawMode drawMode;

    /**
     * Draws a rectangle.
     * @param displayBitmask Display bitmask
     * @param x X position
     * @param y Y position
     * @param w Width of the rectangle
     * @param h Height of the rectangle
     * @param state
     */
    Rectangle(DisplayBitmask displayBitmask, u8 x, u8 y, u8 w, u8 h, DrawMode drawMode)
        : Effect(displayBitmask)
        , x(x)
        , y(y)
        , w(w)
        , h(h)
        , drawMode(drawMode) {
    }

    static void rectangle(u8* buffer, size_t size, u8 x, u8 y, u8 w, u8 h, DrawMode drawMode) {
        if(y > Config::DISPLAY_HEIGHT - 1 || x > size) return;
        if(h > Config::DISPLAY_HEIGHT) h = Config::DISPLAY_HEIGHT;
        if(w > size) w = size;
        for(u8 i = x; i < x + w; i++) {
            for(u8 j = y; j < y + h; j++) {
                SetPixel::setPixel(buffer, size, i, j, drawMode);
            }
        }
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        rectangle(buffer, display->getSize(), x, y, w, h, drawMode);
    }
};