#include "Sequences.h"

using namespace Bitmaps;
using namespace Config::Displays;

/**
 * This is where you define your face animations (Sequences).
 * Check README.txt for an explanation as it is somewhat lengthy.
 */
namespace Sequences {
    // General effects
    Translate* translateEyes = new Translate (EYES, 0, 0);
    Translate* translateEyeLeft = new Translate(EYE_L, 0, -8);
    Translate* translateEyeRight = new Translate(EYE_R, 0, -8);
    Translate* translateNoseLeft = new Translate(NOSE_L, -8, 0);
    Translate* translateNoseRight = new Translate(NOSE_R, -8, 0);
    Translate* translateMouthLeft = new Translate(MOUTH_L, 0, 8);
    Translate* translateMouthRight = new Translate(MOUTH_R, 0, 8);

    Translate* translateAll = new Translate(ALL, 0, 0);

    Glitch* smallGlitch = new Glitch(ALL, 5, 3);
    ParticleEdge* pulseEyes = new ParticleEdge(EYES, 1, 0, 1.9f, 0, 2.0f, 0);
    
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
        {{ {new Mirror(NOSE_L | MOUTH_L)}, {{EYES, Eye::questionMark}, {MOUTH, Mouth::sad}, {NOSE, Nose::neutral}} }},
        "Questioning",
        &Transitions::any
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
            { {mirrorLeft, translateAll, translateEyeRight, winkRight, new EmitParticles(EYE_R, 5, 4, 4, -0.2, -1.0, -0.8, 1.0),}, {}, 350 MILLIS },
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

    // Startup ----------------------------
    #define STARTUP_EFFECTS translateEyeLeft,translateEyeRight,translateNoseLeft,translateNoseRight,translateMouthLeft,translateMouthRight,mirrorLeft
    const f32 STARTUP_TWEEN_SPEED = 750 MILLIS;
    const i32 STARTUP_TWEEN_DELAY = 170 MILLIS;
    Sequence startup {
        {
            { {STARTUP_EFFECTS}, {{EYES, Eye::neutral}, {MOUTH, Mouth::neutral}, {NOSE, Nose::neutral}}, 0},
            { {new Tween<i8>(&(translateEyeLeft->y), -8, 0, STARTUP_TWEEN_SPEED, TWEEN_OUT), STARTUP_EFFECTS}, {}, STARTUP_TWEEN_DELAY },
            { {new Tween<i8>(&(translateEyeRight->y), -8, 0, STARTUP_TWEEN_SPEED, TWEEN_OUT), STARTUP_EFFECTS}, {}, STARTUP_TWEEN_DELAY },
            { {new Tween<i8>(&(translateNoseLeft->x), 8, 0, STARTUP_TWEEN_SPEED, TWEEN_OUT), STARTUP_EFFECTS}, {}, STARTUP_TWEEN_DELAY },
            { {new Tween<i8>(&(translateNoseRight->x), 8, 0, STARTUP_TWEEN_SPEED, TWEEN_OUT), STARTUP_EFFECTS}, {}, STARTUP_TWEEN_DELAY },
            { {new Tween<i8>(&(translateMouthLeft->y), 8, 0, STARTUP_TWEEN_SPEED, TWEEN_OUT), STARTUP_EFFECTS}, {}, STARTUP_TWEEN_DELAY },
            { {new Tween<i8>(&(translateMouthRight->y), 8, 0, STARTUP_TWEEN_SPEED, TWEEN_OUT), STARTUP_EFFECTS}, {}, STARTUP_TWEEN_DELAY }
        },
        "Hello world!",
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
}