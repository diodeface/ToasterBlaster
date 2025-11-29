#pragma once

#include <vector>
#include "Arduino.h"
#include "Typedefs.h"

/**
 * Declare your bitmaps here.
 */

namespace Bitmaps {
    namespace Eye {
        extern Bitmap 
            neutral[16], 
            blushing[16],
            joy[16],
            joyBlush[16],
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
            dizzy1[16],
            batteryCheck[16],
            u[16],
            o[16],
            exclamationPoint[16],
            crying0[16],
            crying1[16];
    };

    namespace Mouth {
        extern Bitmap
            neutral[32],
            sad[32],
            happy[32],
            smile[32],
            smileTongue[32],
            batteryCheck[32],
            w[32],
            smirk[32],
            shaking0[32],
            shaking1[32],
            shaking2[32],
            nope[32];
    };

    namespace Nose {
        extern Bitmap
            neutral[8],
            o[8];
    };

    namespace Other {
        extern Bitmap empty[32];
        extern Bitmap full[32];
        extern Bitmap digits[10][3];
    };

    extern std::vector<Bitmap*> 
        eyeBitmaps,
        mouthBitmaps,
        noseBitmaps;
};