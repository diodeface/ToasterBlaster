#pragma once

#include "Typedefs.h"
#include "Arduino.h"
#include "System/Animation/Sequence/Sequence.h"
#include "Assets/Transitions/Standard.h"

/**
 * This is where you declare your face animations (Sequences).
 * Check README.txt for an explanation as it is quite lengthy.
 */
namespace Sequences {
    extern Sequence 
        // basic
        neutral,
        joy,
        joyBlush,
        blushing,
        angryHappy,
        angry,
        spooked,
        annoyed,
        questioning,
        exclamation,
        dead,
        squinting,
        uwu,
        owo,
        nope,
        
        // animated
        startup,
        dizzy,
        heartEyes,
        wink,
        powerOff,
        batteryCheck,
        crying,

        // miscellaneous
        displayTest,
        none,
        randomize;

    extern std::vector<Sequence> sequences;
}