#pragma once

#include <vector>
#include "Arduino.h"
#include "Utilities/Typedefs.h"

/**
 * Declare your bitmaps here.
 */

namespace Bitmaps {
    namespace Eye {
        extern Bitmap 
            neutral[16], 
            blushing[16],
            joy[16],
            flushed[16],
            dead[16],
            questionMark[16],
            angry[16],
            annoyed[16],
            boop[16],
            heart0[16],
            heart1[16],
            heart2[16],
            dizzy0[16],
            dizzy1[16];
    };

    namespace Mouth {
        extern Bitmap
            neutral[32],
            sad[32],
            happy[32],
            smile[32],
            smileTongue[32];
    };

    namespace Nose {
        extern Bitmap
            neutral[8];
    };

    namespace Other {
        extern Bitmap empty[32];
        extern Bitmap digits[10][3];
    };

    extern std::vector<Bitmap*> 
        eyeBitmaps,
        mouthBitmaps,
        noseBitmaps;
};