#pragma once

#include "Utilities/Typedefs.h"
#include "Arduino.h"
#include "Sequence.h"

/**
 * This is where you declare your face animations (Sequences).
 * Check README.txt for an explanation as it is quite lengthy.
 */
namespace Sequences {
    extern Sequence 
        neutral,
        joy,
        blushing,
        angryHappy,
        angry,
        spooked,
        annoyed,
        questioning,
        wink,
        heartEyes,
        dead,
        dizzy,
        startup,
        squinting,
        randomize;
}