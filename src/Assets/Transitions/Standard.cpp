#include "System.h"
#include "Standard.h"

/**
 * Define your transitions here. Read OverlaySequence.h for more info.
 */

using namespace Config::Displays;

namespace Transitions {
    Translate* translateAll = new Translate (ALL, 0, 0);
    Translate* vTranslate = new Translate(EYES, 0, 0);
    Translate* hTranslate = new Translate(EYES, 0, 0);
    Translate* translateLeft = new Translate(LEFT_SIDE, 0, 0);
    Translate* translateRight = new Translate(RIGHT_SIDE, 0, 0);

    TVStatic* staticAll = new TVStatic(ALL, 0);
    Glitch* glitchAll = new Glitch(ALL, 0, 0);
    Expand* expandAll = new Expand(ALL, 0);
    RandomizeFace* randomizeAll = new RandomizeFace (ALL, 10 MILLIS);
    Earthquake* earthQuakeAll = new Earthquake(ALL, 0, 0);
    
    LambdaEffect* crossfadeMouthNose = new LambdaEffect(MOUTH | NOSE, [](u8* buffer, Display* display) {crossfadeEffect(buffer, display);}, false);

    // Pulse ---------------------------------
    Transition pulse {
        {
            SKIP_FRAME, // skip frame so that particle edge can catch up with the new bitmap
            {{new ParticleEdge(ALL, 1, 0, 50, 0, 50, 0)}, 0},
            END_SEQUENCE
        }
    };
    // ---------------------------------------


    // Slide ---------------------------------
    Transition slide {
        {
            {
                {
                    new Tween<i8>(&(translateLeft->x), 0, -64, 350 MILLIS, TWEEN_IN_BACK), 
                    new Tween<i8>(&(translateRight->x), 0, 64, 350 MILLIS, TWEEN_IN_BACK), 
                    translateLeft, translateRight
                }, 
                500 MILLIS
            },
            {
                {
                    new Tween<i8>(&(translateLeft->x), 32, 0, 1000 MILLIS, TWEEN_OUT_ELASTIC), 
                    new Tween<i8>(&(translateRight->x), -32, 0, 1000 MILLIS, TWEEN_OUT_ELASTIC), 
                    translateLeft, translateRight
                }, 
                1500 MILLIS
            },
            END_SEQUENCE
        }, 2
    };
    // --------------------------------------- 


    // Drop ----------------------------------
    Transition drop {
        {
            {{new Tween<i8>(&(vTranslate->y), 0, 8, 300 MILLIS, TWEEN_IN), vTranslate}, 300 MILLIS},
            {
                {
                    crossfadeMouthNose,
                    new Tween<i8>(&(vTranslate->y), -8, 0, 700 MILLIS, TWEEN_OUT_BOUNCE), vTranslate
                }, 700 MILLIS
            },
            END_SEQUENCE
        }, 2
    };
    // --------------------------------------- 


    // TV Static -----------------------------
    Transition switchChannel {
        {
            {{new Tween<u8>(&(staticAll->intensity), 30, 0, 600 MILLIS, TWEEN_LINEAR), staticAll}, 600 MILLIS},
            END_SEQUENCE
        }
    };
    // --------------------------------------- 


    // Glitch --------------------------------
    Transition glitch {
        {
            {
                {
                    new Tween<u8>(&(glitchAll->intensity), 100, 0, 750 MILLIS, TWEEN_LINEAR), 
                    new Tween<u8>(&(glitchAll->maxShift), 8, 1, 750 MILLIS, TWEEN_LINEAR), 
                    glitchAll
                }, 750 MILLIS
            },
            END_SEQUENCE
        }, 0
    };
    // ---------------------------------------


    // Expand --------------------------------
    Transition expand {
        {
            {{new Tween<u8>(&(expandAll->iterations), 0, 6, 300 MILLIS, TWEEN_IN_EXPONENTIAL), expandAll}, 300 MILLIS}, 
            {{new Tween<u8>(&(expandAll->iterations), 6, 0, 300 MILLIS, TWEEN_OUT_EXPONENTIAL), expandAll}, 300 MILLIS}, 
            END_SEQUENCE
        }, 2
    };
    // ---------------------------------------


    // Explode -------------------------------
    Transition explode {
        {
            {{new LambdaEffect(ALL, [](u8* buffer, Display* display) {
                u8 size = display->getSize();
                for (u8 x = 0; x < size; x++) {
                    for (u8 y = 0; y < Config::DISPLAY_HEIGHT; y++) {
                        display->particleSystem->emit(
                            1, size, x, y,

                            -50, -20,
                            50, 0,

                            0, 60,
                            0, 60,

                            getPixel(buffer, size, x, y)
                        );
                    }
                }
            })}, 0 MILLIS}, 
            END_SEQUENCE
        }, 1
    };
    // ---------------------------------------


    // Doom-like melt ------------------------
    Transition doomMelt {
        {
            {{new LambdaEffect(ALL, [](u8* buffer, Display* display) {
                u8 size = display->getSize();
                for (u8 x = 0; x < size; x++) {
                    q8_8 meltSpeed = random(q8_8(20), q8_8(100));
                    for (u8 y = 0; y < Config::DISPLAY_HEIGHT; y++) {
                        display->particleSystem->emit(
                            1, size, q8_8(x), q8_8(y),

                            q8_8(0), q8_8(0),
                            q8_8(0), q8_8(0),

                            q8_8(0), meltSpeed,
                            q8_8(0), meltSpeed,

                            getPixel(buffer, size, x, y)
                        );
                    }
                }
            })}, 0 MILLIS}, 
            END_SEQUENCE
        }, 1
    };
    // ---------------------------------------


    // Shuffle -------------------------------
    Transition shuffle {
        {
            {
                {
                    new Tween<Timestamp>(&(randomizeAll->period), 10 MILLIS, 250 MILLIS, 1 SECONDS, TWEEN_LINEAR),
                    randomizeAll,
                    mirrorLeft
                }, 1 SECONDS
            },
            END_SEQUENCE
        }, 1
    };
    // ---------------------------------------


    // Earthquake ----------------------------
    Transition earthquake {
        {
            {
                {
                    new Tween<u8>(&(earthQuakeAll->amplitudeX), 8, 0, 500 MILLIS, TWEEN_OUT), 
                    new Tween<u8>(&(earthQuakeAll->amplitudeY), 4, 0, 500 MILLIS, TWEEN_OUT), 
                    earthQuakeAll
                }, 500 MILLIS
            },
            END_SEQUENCE
        }, 1
    };
    // ---------------------------------------


    // Lose power ----------------------------
    Transition losePower {
        {
            {{new LosePower(ALL, 10)}, 100 MILLIS},
            {{new LosePower(ALL, 25)}, 100 MILLIS},
            {{new LosePower(ALL, 50)}, 100 MILLIS},
            {{new LosePower(ALL, 75)}, 100 MILLIS, -1, 3},
            {{new LosePower(ALL, 25)}, 100 MILLIS, -1, 2},
            END_SEQUENCE
        }, 4
    };
    // ---------------------------------------


    // Fizz ----------------------------------
    namespace {
        LambdaEffect* fizzEmitter = new LambdaEffect(ALL, [](u8* buffer, Display* display) {
            u8 size = display->getSize();
            for (u8 i = 0; i < size; i++) {
                if (random(20) > 0) continue;
                display->particleSystem->emit(1, size, i, 7, -10, -80, 10, -40);
            }
        });
    }
    Transition fizz {
        {
            {{new Tween<i8>(&(translateAll->y), 0, -6, 200 MILLIS, TWEEN_IN), translateAll, fizzEmitter}, 200 MILLIS},
            {{new Tween<i8>(&(translateAll->y), 6, 0, 200 MILLIS, TWEEN_OUT), translateAll, fizzEmitter}, 200 MILLIS},
            END_SEQUENCE
        }, 2
    };
    // ---------------------------------------


    // 56k modem -----------------------------
    Transition modem {
        {
            {{new LambdaEffect(ALL, [](u8* buffer, Display* display) {
                u8 size = display->getSize();
                Timestamp delay = random(50 MILLIS, 150 MILLIS);
                for (u8 y = 0; y < Config::DISPLAY_HEIGHT; y++) {
                    for (u8 x = 0; x < size; x++) {
                        display->particleSystem->particles.push_back(
                            {q8_8(x), q8_8(y), q8_8(0), q8_8(0), q8_8(0), q8_8(0), (i32)(delay * (y + 1)), size, DRAW_MODE_0}
                        );
                    }
                }
            })}, 0 MILLIS}, 
            END_SEQUENCE
        }, 1
    };
    // ---------------------------------------
}