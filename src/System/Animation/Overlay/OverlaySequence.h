#pragma once

/**
 * OverlaySequence is a sequence of effects that's used by OverlayPlayer.
 * This is a simple way to create transitions or timed effects like blinking.
 * OverlaySequence works similarly to Sequence, but only effects are available.
 * 
 * OverlaySequence structure:
 * - Effect keyframes (vector <OverlayKeyframe>)
 * - minimum Wait time before starting animation
 * - maximum Wait time before starting animation
 * 
 * Optional parameters:
 * - Transition keyframe index
 *   When using an OverlaySequence as a transition, the keyframe at this index
 *   is where the change happens, otherwise the change is made immediately.
 *
 * Invalid index or next keyframe value of -3 stops the sequence,
 * therefore, it has to be started again with start() or forceStart().
 */

#include "OverlayKeyframe.h"
#include "System/Utilities/Util.h"
#include "System/Drawing/Effects/Effects.h"

struct OverlaySequence {
    std::vector<OverlayKeyframe> keyframes;
    Timestamp minWait, maxWait;
    u32 transitionIndex;

    OverlaySequence(
        const std::vector<OverlayKeyframe>& keyframes, 
        Timestamp minWait, 
        Timestamp maxWait, 
        u32 transitionIndex = 0
    ) 
        : keyframes (keyframes)
        , minWait (minWait)
        , maxWait (maxWait)
        , transitionIndex (transitionIndex) 
    {}

    OverlaySequence(
        const std::vector<OverlayKeyframe>& keyframes, 
        u32 transitionIndex = 0
    ) 
        : OverlaySequence(keyframes, 0, 0, transitionIndex)
    {}
};

typedef OverlaySequence Transition;
static Effect* NULL_EFFECT = new NullEffect();
#define SKIP_FRAME {{NULL_EFFECT}, 0}
#define END_SEQUENCE {{NULL_EFFECT}, 0, 0}