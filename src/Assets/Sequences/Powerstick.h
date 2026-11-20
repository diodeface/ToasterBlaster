#pragma once

#include "Typedefs.h"
#include "Arduino.h"
#include "System/Animation/Sequence/Sequence.h"
#include "Assets/Transitions/Standard.h"

namespace Sequences {
    extern Sequence 
        powerstickStartup,
        powerstickDefault,
        powerstickActivate,
        powerstickVacuumFail,
        powerstickVacuumStart,
        powerstickVacuumEnd1000,
        powerstickVacuumEnd1500,
        powerstickVacuumEnd2000,
        powerstickVacuumEnd4000;
}