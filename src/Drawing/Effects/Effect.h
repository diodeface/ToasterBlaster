#pragma once

#include "Arduino.h"
#include <vector>
#include "Utilities/Util.h"
#include "Drawing/Display.h"

/**
 * Base class for effects.
 * Implement your own constructor and apply function.
 * Apply function gives you access to a display and requested buffer to update.
 */
class Effect {
  public:
    DisplayBitmask displayBitmask;  // which displays to apply the bitmap to
    bool applyOnRedraw;             // true if the effect should be applied on each redraw
                                    // this is so that particles don't keep emitting as they are being redrawn
    bool applied;                   // set this to true after effect is applied

    Effect(DisplayBitmask displayBitmask, bool applyOnRedraw = true)
        : displayBitmask(displayBitmask)
        , applyOnRedraw(applyOnRedraw)
        , applied(false) {
    }

    virtual ~Effect() {
    }

    // called before effect is applied for the first time
    virtual void reset() {
        applied = false;
    };

    virtual void apply(u8* buffer, Display* display) {
        applied = true;
    }
};