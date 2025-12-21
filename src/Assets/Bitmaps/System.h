#pragma once

#include "Arduino.h"
#include "Typedefs.h"

namespace Bitmaps {
    namespace System {
        const u8 
            splash1Width = 88,
            splash1Height = 55,
            splash2Width = 30,     // padded to 32
            splash2Height = 20,
            visorWidth = 54,         // padded to 56
            visorHeight = 38;
        
        extern Bitmap 
            splash1[], 
            splash2[],
            visor[];

        extern Bitmap 
            digits[10][3];
    }
}