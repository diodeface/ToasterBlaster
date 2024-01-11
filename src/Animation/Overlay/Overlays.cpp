#include "Overlays.h"

/**
 * Declare your overlays here. Read OverlaySequence.h for more info.
 * 
 * Remember to set interruptable boolean on tweens to false to make sure they
 * don't get cleared on a sequence or controller change.
 */

using namespace Config::Displays;

namespace Overlays {
    // Boop ----------------------------------
    Effect* boopEffects[5] {
        new DrawBitmap(EYES, Bitmaps::Eye::boop, 16),
        new DrawBitmap(MOUTH_L, Bitmaps::Mouth::happy, 32),
        new DrawBitmap(MOUTH_R, Bitmaps::Mouth::happy, 32),
        new DrawBitmap(NOSE, Bitmaps::Nose::neutral, 8),
        new Mirror(LEFT_SIDE)
    };
    #define BOOP_EFFECTS boopEffects[0], boopEffects[1], boopEffects[2], boopEffects[3], boopEffects[4]
    Glitch* boopGlitch = new Glitch(ALL, 0, 4);
    OverlaySequence boop {
        {
            {
                {
                    new Tween<u8>(&(boopGlitch->intensity), 100, 0, 750 MILLIS, TWEEN_LINEAR, false), 
                    new Tween<u8>(&(boopGlitch->maxShift), 8, 1, 750 MILLIS, TWEEN_LINEAR, false), 
                    BOOP_EFFECTS,
                    boopGlitch
                }, 750 MILLIS
            },
            {{BOOP_EFFECTS}, UINT_MAX, -2}
        }
    };
    // ---------------------------------------

    // EyeBlink class related overlays
    namespace EyeBlink {
        Blink* blinkEyes = new Blink(EYES,  0);
        Translate* translateEyes = new Translate(EYES, 0, 0);
        Expand* expandEyes = new Expand(EYES, 0);

        OverlaySequence closing {
            {
                {
                    {
                        new Tween<u8>(&(blinkEyes->amount), 0, 5, 100 MILLIS, TWEEN_LINEAR, false), 
                        new Tween<i8>(&(translateEyes->y), 0, 3, 100 MILLIS, TWEEN_LINEAR, false),
                        new Tween<u8>(&(expandEyes->iterations), 0, 3, 100 MILLIS, TWEEN_LINEAR, false),
                        translateEyes, expandEyes, blinkEyes 
                    }, 100 MILLIS
                },
                {{translateEyes, expandEyes, blinkEyes}, 0, 0}     // todo: fix bug in overlayplayer where the last keyframe is only displayed once before stopping requiring dirty hacks like this :)
            }
        };

        OverlaySequence closed {
            {
                {
                    {new Clear(EYES)}, UINT_MAX, -2
                }
            }
        };

        OverlaySequence opening {
            {
                {
                    {
                        new Tween<u8>(&(blinkEyes->amount), 5, 0, 100 MILLIS, TWEEN_LINEAR, false), 
                        new Tween<i8>(&(translateEyes->y), 3, 0, 100 MILLIS, TWEEN_LINEAR, false),
                        new Tween<u8>(&(expandEyes->iterations), 3, 0, 100 MILLIS, TWEEN_LINEAR, false),
                        translateEyes, expandEyes, blinkEyes 
                    }, 100 MILLIS
                },
                END_SEQUENCE
            }
        };
    }
}