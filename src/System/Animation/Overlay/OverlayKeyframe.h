#pragma once

#include "System/Animation/Sequence/Keyframe.h"
#include "config.h"
#include <vector>

struct OverlayKeyframe {
    std::vector<Effect*> effects;
    i16 next;
    i16 repeat;
    Timestamp duration;

    OverlayKeyframe(
        const std::vector<Effect*>& effects = {}, 
        Timestamp duration = Config::DEFAULT_OVERLAY_KEYFRAME_DURATION, 
        i16 next = -1, 
        i16 repeat = 0
    ) 
        : effects(effects)
        , duration(duration)
        , next(next)
        , repeat(repeat) 
    {}
};