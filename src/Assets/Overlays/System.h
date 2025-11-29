#pragma once

/**
 * System required overlays.
 */

#include "Typedefs.h"
#include "System/Animation/Overlay/OverlaySequence.h"

namespace Overlays {
    namespace Boop {
        extern OverlaySequence 
        boop;
    }
    
    namespace EyeBlink {
        extern OverlaySequence 
            closing, 
            closed, 
            opening;
    }
}