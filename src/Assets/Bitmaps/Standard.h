#pragma once
// This header file is automatically generated during the build process!
// To modify, edit the original png files and build again.

#include <vector>
#include "Arduino.h"
#include "Typedefs.h"

namespace Bitmaps {
    extern const char* BITMAP_GENERATION_TIMESTAMP;

    namespace Eye {
        extern Bitmap
            angry[16],
            annoyed[16],
            batteryCheck[16],
            blushing[16],
            boop[16],
            crying0[16],
            crying1[16],
            dead[16],
            dizzy0[16],
            dizzy1[16],
            exclamationPoint[16],
            flushed[16],
            heart0[16],
            heart1[16],
            heart2[16],
            joy[16],
            joyBlush[16],
            neutral[16],
            o[16],
            questionMark[16],
            u[16];
    }

    namespace Mouth {
        extern Bitmap
            batteryCheck[32],
            happy[32],
            neutral[32],
            nope[32],
            sad[32],
            shaking0[32],
            shaking1[32],
            shaking2[32],
            smile[32],
            smileTongue[32],
            smirk[32],
            w[32];
    }

    namespace Nose {
        extern Bitmap
            neutral[8],
            o[8];
    }

    namespace Other {
        extern Bitmap
            empty[32],
            full[32];
    }

    extern std::vector<Bitmap*>
        EyeBitmaps,
        MouthBitmaps,
        NoseBitmaps,
        OtherBitmaps;
}