#include "Powerstick.h"

using namespace Bitmaps;
using namespace Config::Displays;

/**
 * This is where you define your face animations (Sequences).
 * Check README.txt for an explanation as it is somewhat lengthy.
 */
namespace Sequences {
    // General effects
    Translate* psTranslateEyeLeft = new Translate(EYE_L, 0, 0);
    Translate* psTranslateEyeRight = new Translate(EYE_R, 0, 0);
    Translate* psTranslateEyes = new Translate(EYES, 0, 0);
    Glitch* psGlitchEyes = new Glitch(EYES, 0, 0);
    Earthquake* psEarthquakeEyes = new Earthquake(EYES, 0, 0);
    Expand* psExpandEyes = new Expand(EYES, 0);

    // TODO: eye dot moving around randomly
    Sequence powerstickDefault {
        {
            {   
                {mirrorLeft},{{EYES, Eye::powerstick}}
            },
        },
        "Powerstick!",
        &Transitions::glitch
    };

    Sequence powerstickStartVacuuming {
        {
            {
                { mirrorLeft, psTranslateEyes, new Tween<i8>(&(psTranslateEyes->x), 0, -4, 220 MILLIS, TWEEN_OUT),},
                {{EYES, Eye::o}}, 220 MILLIS,
            },
            {
                { mirrorLeft, psTranslateEyes, new Tween<i8>(&(psTranslateEyes->x), -4, 4, 200 MILLIS, TWEEN_OUT),},
                {}, 200 MILLIS,
            },
            {
                { mirrorLeft, psTranslateEyes, new Tween<i8>(&(psTranslateEyes->x), 4, -4, 220 MILLIS, TWEEN_OUT),},
                {}, 220 MILLIS,
            },
            {
                { mirrorLeft, psTranslateEyes, new Tween<i8>(&(psTranslateEyes->x), -4, 0, 250 MILLIS, TWEEN_OUT),},
                {}, 250 MILLIS,
            },
            {
                {   
                    mirrorLeft, psEarthquakeEyes, psGlitchEyes,
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeX), 0, 2, 1000 MILLIS, TWEEN_LINEAR), 
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeY), 0, 1, 2500 MILLIS, TWEEN_IN),               
                },
                {{EYES, Eye::boop}}, 2500 MILLIS,
            },
            {
                { mirrorLeft, psEarthquakeEyes, psGlitchEyes },
                {}, Config::FRAMETIME, -2
            }
        },
        "Vacuuming",
        nullptr
    };

    // TODO: end vacuuming sequence
    Sequence powerstickEndVacuuming {
        {
            {   
                {mirrorLeft},{{EYES, Eye::powerstick}}
            },
        },
        "End vacuuming",
        nullptr
    };

    // TODO: bug in the tweening system causes tweened variables to inhibit previous values for one frame, tween in keyframe 0 is a workaround
    Sequence powerstickError {
        {
            {{new Tween<u8>(&(psExpandEyes->iterations), 0, 0, 0 MILLIS, TWEEN_IN)},{{EYES, Eye::exclamationPoint}}, 114 MILLIS},
            {{},{{EYES, Other::empty}}, 35 MILLIS},
            {{},{{EYES, Eye::exclamationPoint}}, 184 MILLIS},
            {{},{{EYES, Other::empty}}, 32 MILLIS},
            {{},{{EYES, Eye::exclamationPoint}}, 2000 MILLIS},
            {{psExpandEyes, new Tween<u8>(&(psExpandEyes->iterations), 0, 5, 250 MILLIS, TWEEN_LINEAR)},{{}}, 250 MILLIS},
            {{mirrorLeft, },{{EYES, Eye::powerstick}}, 1000 MILLIS},
        },
        "Error",
        nullptr
    };
}