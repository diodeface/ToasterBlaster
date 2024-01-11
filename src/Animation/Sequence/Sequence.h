#pragma once

#include "Arduino.h"
#include "Keyframe.h"
#include "Animation/Overlay/Transitions.h"
#include "Animation/Overlay/Overlays.h"

struct Sequence {
    std::vector<Keyframe> keyframes;
    char name[16];
    OverlaySequence* transition;
    Timestamp minWait, maxWait;

    /**
     * A sequence of keyframes, used by SequencePlayer.
     * @param keyframes Vector of keyframes for this sequence.
     * @param name Name of the sequence (15 characters).
     * @param transition Transition (OverlaySequence) to play when transitioning to this sequence.
     * @param minWait Minimum wait time before the sequence is repeated when the last keyframe is reached.
     * @param maxWait Maximum wait time before the sequence is repeated when the last keyframe is reached.
     */
    Sequence(
        const std::vector<Keyframe>& keyframes = {}, 
        const char* name = "No name",
        OverlaySequence* transition = nullptr, 
        Timestamp minWait = 0, 
        Timestamp maxWait = 0
    )
        : keyframes(keyframes)
        , minWait(minWait)
        , maxWait(maxWait)
        , transition(transition) {
        strncpy(this->name, name, 15);
    }
};