#pragma once

/**
 * System required transitions.
 */

#include "Typedefs.h"
#include "System/Animation/Overlay/OverlaySequence.h"
#include "System/Drawing/Particles/ParticleSystem.h"

using namespace Config::Displays;


namespace Transitions {
    void crossfadeEffect(u8* buffer, Display* display);
    extern Transition
        any,
        blink,
        crossfade;
}