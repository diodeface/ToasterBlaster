#pragma once

#include "Typedefs.h"
#include "Arduino.h"
#include "System/Animation/Sequence/Sequence.h"
#include "Assets/Transitions/Standard.h"

namespace Sequences {
    extern Sequence 
        powerstickDefault,
        powerstickError,
        powerstickStartVacuuming,
        powerstickEndVacuuming;
}