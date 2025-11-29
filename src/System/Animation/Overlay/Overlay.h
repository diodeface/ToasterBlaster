#pragma once

/**
 * Overlays have an ID, used by addOverlay and removeOverlay functions in
 * DisplayManager to identify which overlays are to be removed.

 * Overlay players assign their own ID starting with globalId in OverlayPlayer.cpp,
 * therefore IDs 0 - 31 are free to use by whatever custom overlays you write.
 * Currently used IDs:
 * 0 - Animated Mouth
 *
 * Note that standard overlays and matrix overlays don't share IDs. This is
 * intentional as matrix overlays are never meant to be removed, their use is
 * to add permanent rotation and mirroring effects that shold not be visible 
 * on the HUD.
 */

#include "Typedefs.h"
#include "OverlaySequence.h"

struct Overlay {
    Effect* effect;
    u8 id;
};