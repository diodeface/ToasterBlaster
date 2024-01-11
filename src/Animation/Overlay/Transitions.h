#pragma once

/**
 * Declare your transitions here. Read OverlaySequence.h for more info.
 */

#include "Utilities/Typedefs.h"
#include "OverlaySequence.h"

namespace Transitions {
    extern Transition 
        any,
        blink,
        pulse,
        slide,
        drop,
        switchChannel,
        glitch,
        expand,
        explode,
        doomMelt,
        crossfade,
        shuffle,
        losePower,
        fizz;
}