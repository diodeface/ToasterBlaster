#include "System.h"

/**
 * System required transitions.
 */

using namespace Config::Displays;

namespace Transitions {
    void crossfadeEffect (u8* buffer, Display* display) {{
        u8 size = display->getSize();
        for (u8 x = 0; x < size; x++) {
            for (u8 y = 0; y < Config::DISPLAY_HEIGHT; y++) {
                display->particleSystem->particles.push_back(
                    {q8_8(x), q8_8(y), q8_8(0), q8_8(0), q8_8(0), q8_8(0), (i32)random(150 MILLIS), size, getPixel(buffer, size, x, y)}
                );
            }
        }
    }}

    // Blink ---------------------------------
    Blink* boop_blinkEyes = new Blink(EYES, 0);
    Expand* boop_expandEyes = new Expand(EYES, 0);
    Translate* boop_vTranslate = new Translate(EYES, 0, 0);
    LambdaEffect* boop_crossfadeMouthNose = new LambdaEffect(MOUTH | NOSE, [](u8* buffer, Display* display) {crossfadeEffect(buffer, display);}, false);
    Transition blink {
        {
            {
                {
                    new Tween<u8>(&(boop_blinkEyes->amount), 0, 8, 100 MILLIS, TWEEN_LINEAR), 
                    new Tween<i8>(&(boop_vTranslate->y), 0, 3, 100 MILLIS, TWEEN_LINEAR),
                    new Tween<u8>(&(boop_expandEyes->iterations), 0, 3, 100 MILLIS, TWEEN_LINEAR, false),
                    boop_vTranslate, boop_expandEyes, boop_blinkEyes 
                }, 100 MILLIS
            },
            {
                {
                    boop_crossfadeMouthNose,
                    new Tween<u8>(&(boop_blinkEyes->amount), 8, 0, 100 MILLIS, TWEEN_LINEAR), 
                    new Tween<i8>(&(boop_vTranslate->y), 3, 0, 100 MILLIS, TWEEN_LINEAR),
                    new Tween<u8>(&(boop_expandEyes->iterations), 3, 0, 100 MILLIS, TWEEN_LINEAR, false),
                    boop_vTranslate, boop_expandEyes, boop_blinkEyes 
                }, 100 MILLIS
            },
            END_SEQUENCE
        }, 2
    };
    // ---------------------------------------


    // Crossfade -----------------------------
    Transition crossfade {
        {
            {{new LambdaEffect(ALL, [](u8* buffer, Display* display) {crossfadeEffect(buffer, display);})}, 0 MILLIS}, 
            END_SEQUENCE
        }, 1
    };
    // ---------------------------------------


    // Any -----------------------------------
    // Choose randomly from the common and rare transitions provided to a sequence player
    Transition any {{}};
    // ---------------------------------------
}