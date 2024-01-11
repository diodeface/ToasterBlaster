#pragma once

/**
 * Define your overlays here. Read OverlaySequence.h for more info.
 */

#include "Utilities/Typedefs.h"
#include "OverlaySequence.h"

namespace Overlays {
    extern OverlaySequence 
        boop;

    namespace EyeBlink {
        extern OverlaySequence 
            closing, 
            closed, 
            opening;
    }
}