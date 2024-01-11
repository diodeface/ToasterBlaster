#pragma once

#include "Utilities/Typedefs.h"
#include "Effect.h"
#include "Mirror.h"
#include "VerticalMirror.h"

class Rotate180 : public Effect {
  public:
    /**
     * Rotate the buffer by 180 degrees.
     * @param displayBitmask Display bitmask
     */
    Rotate180(DisplayBitmask displayBitmask = Config::Displays::ALL)
        : Effect(displayBitmask) {
    }

    static void rotate180(u8* buffer, size_t size) {
        VerticalMirror::verticalMirror(buffer, size);
        Mirror::mirror(buffer, size);
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        rotate180(buffer, display->getSize());
    }
};