#pragma once

/**
 * Declare your transitions here. Read OverlaySequence.h for more info.
 */

#include "Typedefs.h"
#include "System/Animation/Overlay/OverlaySequence.h"
#include "System/Drawing/Particles/ParticleSystem.h"

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
        earthquake,
        losePower,
        fizz,
        modem;
}