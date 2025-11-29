#include "Standard.h"

using namespace Bitmaps;
using namespace Config::Displays;

/**
 * This is where you define your face animations (Sequences).
 * Check README.txt for an explanation as it is somewhat lengthy.
 */
namespace Sequences {
    // General effects
    Translate* translateEyeLeft = new Translate(EYE_L, 0, -8);
    Translate* translateEyeRight = new Translate(EYE_R, 0, -8);
    Translate* translateNoseLeft = new Translate(NOSE_L, -8, 0);
    Translate* translateNoseRight = new Translate(NOSE_R, -8, 0);
    Translate* translateMouthLeft = new Translate(MOUTH_L, 0, 8);
    Translate* translateMouthRight = new Translate(MOUTH_R, 0, 8);
    Translate* translateEyes = new Translate (EYES, 0, 0);
    Translate* translateAll = new Translate(ALL, 0, 0);
    Glitch* glitchAll = new Glitch(ALL, 0, 0);
    TVStatic* tvStaticAll = new TVStatic(ALL, 0);
    Earthquake* earthquakeAll = new Earthquake(ALL, 0, 0);
    Glitch* smallGlitch = new Glitch(ALL, 5, 3);
    LosePower* losePowerAll = new LosePower(ALL, 0);
    ParticleEdge* pulseEyes = new ParticleEdge(EYES, 1, 0, 90, 0, 90, 0);
    
    // Simple faces ---------------------------
    Sequence neutral {
        // keyframes
        {
            // keyframe 0
            {   
                // effect pointers
                {mirrorLeft},
                // bitmaps
                {
                    {EYES, Eye::neutral},
                    {MOUTH, Mouth::neutral},
                    {NOSE, Nose::neutral},
                }
            },
        },
        "Neutral",
        &Transitions::any
    };

    Sequence joy {
        {{ {mirrorLeft}, {{EYES, Eye::joy}, {MOUTH, Mouth::happy}, {NOSE, Nose::neutral}} }},
        "Joy",
        &Transitions::any
    };

    Sequence joyBlush {
        {{ {mirrorLeft}, {{EYES, Eye::joyBlush}, {MOUTH, Mouth::happy}, {NOSE, Nose::neutral}} }},
        "Joy blush",
        &Transitions::any
    };

    Sequence blushing {
        {{ {mirrorLeft}, {{EYES, Eye::blushing}, {MOUTH, Mouth::sad}, {NOSE, Nose::neutral}} }},
        "Blushing",
        &Transitions::any
    };

    Sequence angry {
        {{ {flipMouth, mirrorLeft}, {{EYES, Eye::angry}, {MOUTH, Mouth::sad}, {NOSE, Nose::neutral}} }},
        "Angry",
        &Transitions::any
    };

    Sequence angryHappy {
        {{ {mirrorLeft}, {{EYES, Eye::angry}, {MOUTH, Mouth::neutral}, {NOSE, Nose::neutral}} }},
        "AngryHappy",
        &Transitions::any
    };

    Sequence annoyed {
        {{ {flipMouth, mirrorLeft}, {{EYES, Eye::annoyed}, {MOUTH, Mouth::sad}, {NOSE, Nose::neutral}} }},
        "Annoyed",
        &Transitions::any
    };

    Sequence spooked {
        {{ {mirrorLeft}, {{EYES, Eye::flushed}, {MOUTH, Mouth::sad}, {NOSE, Nose::neutral}} }},
        "Spooked",
        &Transitions::any
    };  

    Sequence squinting {
        {{ {mirrorLeft}, {{EYES, Eye::boop}, {MOUTH, Mouth::happy}, {NOSE, Nose::neutral}} }},
        "Squinting",
        &Transitions::any
    };  

    Sequence questioning {
        {{ {new Mirror(NOSE_L | MOUTH_L)}, {{EYES, Eye::questionMark}, {MOUTH, Mouth::smirk}, {NOSE, Nose::neutral}} }},
        "Questioning",
        &Transitions::drop
    };

    Sequence exclamation {
        {
            { {mirrorLeft}, {{EYES, Eye::exclamationPoint}, {MOUTH, Mouth::smirk}, {NOSE, Nose::neutral}}, 500 MILLIS },
            { {mirrorLeft}, {{EYES, Other::empty}, {MOUTH, Mouth::smirk}, {NOSE, Nose::neutral}}, 200 MILLIS, 0 }
        },
        "Exclamation",
        &Transitions::drop
    };

    Sequence uwu {
        {{ {mirrorLeft}, {{EYES, Eye::u}, {MOUTH, Mouth::w}, {NOSE, Nose::neutral}} }},
        "UWU",
        &Transitions::drop
    };

    Sequence owo {
        {{ {mirrorLeft}, {{EYES, Eye::o}, {MOUTH, Mouth::w}, {NOSE, Nose::neutral}} }},
        "OWO",
        &Transitions::earthquake
    };

    Sequence nope {
        {
            { {}, {{EYES, Mouth::nope}, {MOUTH, Mouth::nope}, {NOSE_R, Mouth::nope}, {NOSE_L, Nose::o}}, 500 MILLIS },
            { {}, {{ALL, Other::empty}}, 100 MILLIS, 0 }
        },
        "NOPE",
        &Transitions::earthquake
    };
    // ---------------------------------------

    // Wink ----------------------------------
    Blink* winkRight = new Blink(EYE_R, 0);
    Sequence wink {
        {
            { {mirrorLeft}, { {EYES, Eye::neutral}, {MOUTH, Mouth::neutral}, {NOSE, Nose::neutral} }, 150 MILLIS },
            { {mirrorLeft, new Tween<i8>(&(translateAll->y), 0, -1, 100 MILLIS, TWEEN_OUT)}, {}, 0 MILLIS },
            { {mirrorLeft, translateAll}, {}, 100 MILLIS },
            { {
                mirrorLeft, translateAll, translateEyeRight, winkRight, 
                new Tween<i8>(&(translateAll->y), -1, 1, 250 MILLIS, TWEEN_IN), 
                new Tween<i8>(&(translateEyeRight->y), 0, 2, 250 MILLIS, TWEEN_IN), 
                new Tween<u8>(&(winkRight->amount), 0, 4, 250 MILLIS, TWEEN_IN),
               }, { {MOUTH, Mouth::smile}}, 0 MILLIS },
            { {mirrorLeft, translateAll, translateEyeRight, winkRight}, {}, 150 MILLIS },
            { {mirrorLeft, translateAll, translateEyeRight, winkRight, new EmitParticles(EYE_R, 5, 4, 4, -40, -20, -10, 20),}, {}, 350 MILLIS },
            { {
                mirrorLeft, translateAll, translateEyeRight, winkRight, 
                new Tween<i8>(&(translateAll->y), 1, 0, 250 MILLIS, TWEEN_IN_OUT), 
                new Tween<i8>(&(translateEyeRight->y), 1, 0, 250 MILLIS, TWEEN_IN_OUT), 
                new Tween<u8>(&(winkRight->amount), 4, 0, 250 MILLIS, TWEEN_IN_OUT)
                }, {}, 0 MILLIS },
            { {mirrorLeft, translateAll, translateEyeRight, winkRight}, {}, 250 MILLIS },
            { {mirrorLeft}, {} }
        },
        "Wink",
        &Transitions::crossfade
    };
    // ---------------------------------------

    // Heart eyes ----------------------------
    Sequence heartEyes {
        {
            { {mirrorLeft}, { {EYES, Eye::heart0}, {MOUTH, Mouth::smile}, {NOSE, Nose::neutral} }, 400 MILLIS },
            { {mirrorLeft}, {{EYES, Eye::heart1}}, 120 MILLIS },
            { {mirrorLeft, pulseEyes}, {{EYES, Eye::heart2}}, 180 MILLIS },
            { {mirrorLeft}, {{EYES, Eye::heart1}}, 120 MILLIS },
            { {mirrorLeft, pulseEyes}, {{EYES, Eye::heart2}}, 140 MILLIS },
            { {mirrorLeft}, {{EYES, Eye::heart1}}, 140 MILLIS, 0}
        },
        "Heart Eyes",
        &Transitions::any
    };
    // ---------------------------------------

    // Dead ----------------------------------
    Sequence dead {
        {
            { 
                {
                    mirrorLeft,
                    smallGlitch
                }, 
                {
                    {EYES, Eye::dead},
                    {MOUTH_L, Mouth::smile},
                    {MOUTH_R, Mouth::smileTongue},
                    {NOSE, Nose::neutral},
                },
                33 MILLIS,
                0
            }
        },
        "Dead",
        &Transitions::glitch
    };
    // ---------------------------------------

    // Power off -----------------------------
    Sequence powerOff {
        {
            { 
                {

                }, 
                {
                    {ALL, Other::empty}
                }
            },
        },
        "Power off",
        &Transitions::losePower
    };
    // ---------------------------------------

    // Battery check -------------------------
    Sequence batteryCheck {
        {
            
            { 
                {}, 
                {{NOSE, Other::empty}, {EYES, Eye::batteryCheck}, {MOUTH, Mouth::batteryCheck}},
                250 MILLIS,
            },  
            { 
                {}, 
                {{ALL, Other::empty}},
                250 MILLIS,
                0
            },          
        },
        "BatteryCheck",
        nullptr
    };
    // ---------------------------------------

    // Crying ----------------------------
    Sequence crying {
        {
            { {new Tween<i8>(&(translateEyes->x), -1, 1, 200 MILLIS, TWEEN_LINEAR), mirrorLeft, translateEyes, new EmitParticles(EYES, 1, 7, 5, -7, -10, 7, 0, 0, 10, 0, 40, DRAW_MODE_XOR)}, { {EYES, Eye::crying0}, {MOUTH, Mouth::shaking0}, {NOSE, Nose::neutral} }, 100 MILLIS },
            { {mirrorLeft, translateEyes}, { {MOUTH, Mouth::shaking1} }, 100 MILLIS },
            { { mirrorLeft, translateEyes}, { {MOUTH, Mouth::shaking2} }, 100 MILLIS },
            { {new Tween<i8>(&(translateEyes->x), 1, -1, 200 MILLIS, TWEEN_LINEAR), mirrorLeft, translateEyes, flipMouth, new EmitParticles(EYES, 1, 9, 5, -7, -10, 7, 0,  0, 10, 0, 40, DRAW_MODE_XOR)}, { {EYES, Eye::crying1}, {MOUTH, Mouth::shaking0} }, 100 MILLIS },
            { {mirrorLeft, translateEyes, flipMouth},  { {MOUTH, Mouth::shaking1} }, 100 MILLIS },
            { {mirrorLeft, translateEyes, flipMouth},  { {MOUTH, Mouth::shaking2} }, 100 MILLIS, 0 },
        },
        "Crying",
        &Transitions::crossfade
    };
    // ---------------------------------------

    // Dizzy ---------------------------------
    Sequence dizzy {
        {
            { { new Tween<i8>(&(translateEyes->x), -1, 1, 300 MILLIS, TWEEN_LINEAR), mirrorLeft, flipLeftEye, smallGlitch, translateEyes }, {{EYES, Eye::dizzy0}, {MOUTH_L, Mouth::smile}, {MOUTH_R, Mouth::smileTongue}, {NOSE, Nose::neutral}}, 75 MILLIS},
            { { mirrorLeft, flipLeftEye, smallGlitch, translateEyes }, {{EYES, Eye::dizzy1}}, 75 MILLIS},
            { { mirrorLeft, flipLeftEye, rotateEyes270, smallGlitch, translateEyes }, {{EYES, Eye::dizzy0}}, 75 MILLIS},
            { { mirrorLeft, flipLeftEye, rotateEyes270, smallGlitch, translateEyes }, {{EYES, Eye::dizzy1}}, 75 MILLIS},
            { { new Tween<i8>(&(translateEyes->x), 1, -1, 300 MILLIS, TWEEN_LINEAR), mirrorLeft, flipLeftEye, rotateEyes180, smallGlitch, translateEyes }, {{EYES, Eye::dizzy0}}, 75 MILLIS},
            { { mirrorLeft, flipLeftEye, rotateEyes180, smallGlitch, translateEyes }, {{EYES, Eye::dizzy1}}, 75 MILLIS},
            { { mirrorLeft, flipLeftEye, rotateEyes90, smallGlitch, translateEyes }, {{EYES, Eye::dizzy0}}, 75 MILLIS},
            { { mirrorLeft, flipLeftEye, rotateEyes90, smallGlitch, translateEyes }, {{EYES, Eye::dizzy1}}, 75 MILLIS, 0}
        },
        "Dizzy!",
        &Transitions::glitch
    };
    // ---------------------------------------

    // Randomize -----------------------------
    Sequence randomize {
        {{{new RandomizeFace (ALL | BITMAPBUFFER, 100 MILLIS), mirrorLeft}, {{EYES, Eye::questionMark}}}, { {mirrorLeft} }},
        "Randomize!",
        &Transitions::shuffle
    };
    // ---------------------------------------

    // Startup ----------------------------
    void startupWipe (Display* display, bool direction) {
        u8 size = display->getSize();
        for (u8 x = 0; x < size; x++) {
            for (u8 y = 0; y < Config::DISPLAY_HEIGHT; y++) {
                display->particleSystem->particles.push_back(
                    {direction ? q8_8(x) : q8_8(size - 1 - x), q8_8(y), q8_8(0), q8_8(0), q8_8(0), q8_8(0), (i32)(x * 30 MILLIS)+random(250 MILLIS), size, DRAW_MODE_0}
                );
            }
        }
    }

    Sequence startup {
        {
            {   
                { 
                    new LambdaEffect(LEFT_SIDE, [](u8* buffer, Display* display) {startupWipe(display, false);}), 
                    new LambdaEffect(RIGHT_SIDE, [](u8* buffer, Display* display) {startupWipe(display, true);}),
                }, {{ALL, Other::empty}}, 0
            },
            { 
                {mirrorLeft}, {{EYES, Eye::neutral}, {MOUTH, Mouth::neutral}, {NOSE, Nose::neutral}}, 0
            },
        },
        "Hello world!",
        &Transitions::crossfade
    };
    // ---------------------------------------

    // Display test --------------------------
    Sequence displayTest {
        {{{}, {{ALL, Other::full}}}},
        "Display test"
    };
    // ---------------------------------------

    // empty ---------------------------------
    Sequence none {
        {{{}, {{ALL, Other::empty}}}},
        "none"
    };
    // ---------------------------------------

    // Sequence list, used by other components
    std::vector<Sequence> sequences {
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
        powerOff,
        batteryCheck,
        dizzy,
        startup,
        squinting,
        displayTest,
        randomize
    };
}