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

    // TODO: redo vacuum end and fail sequences as overlays
    // TODO: eye dot moving around randomly, sleep animation after 5 minutes
    Sequence powerstickDefault {
        {
            {   
                {mirrorLeft},{{EYES, Eye::powerstick}}
            },
        },
        "Powerstick!",
        &Transitions::glitch
    };

    Sequence powerstickVacuumStart {
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
                    mirrorLeft, psEarthquakeEyes,
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeX), 0, 2, 1000 MILLIS, TWEEN_LINEAR), 
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeY), 0, 1, 2500 MILLIS, TWEEN_IN),               
                },
                {{EYES, Eye::boop}}, 2500 MILLIS,
            },
            {
                { mirrorLeft, psEarthquakeEyes },
                {}, Config::FRAMETIME, -2
            }
        },
        "Vacuuming",
        &Transitions::crossfade
    };

    Sequence powerstickVacuumEnd1000 {
        {
            {   
                {
                    mirrorLeft, psEarthquakeEyes, //psGlitchEyes,
                    //new Tween<u8>(&(psGlitchEyes->intensity), 50, 0, 360 MILLIS, TWEEN_LINEAR), 
                    //new Tween<u8>(&(psGlitchEyes->maxShift), 2, 1, 360 MILLIS, TWEEN_LINEAR), 
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeY), 0, 0, 0 MILLIS, TWEEN_LINEAR),
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeX), 1, 0.4f, 360 MILLIS, TWEEN_LINEAR)
                },
                {{EYES, Eye::boop}}, 360 MILLIS
            },
            {   
                {
                    mirrorLeft
                },
                {{EYES, Eye::powerstick}}
            },
        },
        "End 1000",
        nullptr
    };

    Sequence powerstickVacuumEnd1500 {
        {
            {   
                {
                    mirrorLeft, psEarthquakeEyes, //psGlitchEyes,
                    //new Tween<u8>(&(psGlitchEyes->intensity), 60, 0, 640 MILLIS, TWEEN_LINEAR), 
                    //new Tween<u8>(&(psGlitchEyes->maxShift), 3, 1, 640 MILLIS, TWEEN_LINEAR), 
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeY), 0, 0, 0 MILLIS, TWEEN_LINEAR),
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeX), 1, 0.4f, 640 MILLIS, TWEEN_LINEAR)
                },
                {{EYES, Eye::boop}}, 640 MILLIS
            },
            {   
                {
                    mirrorLeft
                },
                {{EYES, Eye::powerstick}}
            },
        },
        "End 1500",
        nullptr
    };

    Sequence powerstickVacuumEnd2000 {
        {
            {   
                {
                    mirrorLeft, psEarthquakeEyes, //psGlitchEyes,
                    //new Tween<u8>(&(psGlitchEyes->intensity), 75, 0, 1550 MILLIS, TWEEN_LINEAR), 
                    //new Tween<u8>(&(psGlitchEyes->maxShift), 3, 1, 1550 MILLIS, TWEEN_LINEAR), 
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeY), 1, 0.25f, 1550 MILLIS, TWEEN_LINEAR),
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeX), 2, 0.4f, 1550 MILLIS, TWEEN_LINEAR)
                },
                {{EYES, Eye::boop}}, 1550 MILLIS
            },
            {   
                {
                    mirrorLeft
                },
                {{EYES, Eye::powerstick}}
            },
        },
        "End 2000",
        nullptr
    };

    Sequence powerstickVacuumEnd4000 {
        {
            {   
                {
                    mirrorLeft, psEarthquakeEyes, //psGlitchEyes,
                    //new Tween<u8>(&(psGlitchEyes->intensity), 90, 0, 2400 MILLIS, TWEEN_LINEAR), 
                    //new Tween<u8>(&(psGlitchEyes->maxShift), 3, 1, 2400 MILLIS, TWEEN_LINEAR), 
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeY), 1, 0.25f, 2400 MILLIS, TWEEN_LINEAR),
                    new Tween<u8>(&(psEarthquakeEyes->amplitudeX), 2, 0.4f, 2400 MILLIS, TWEEN_LINEAR)
                },
                {{EYES, Eye::boop}}, 2400 MILLIS
            },
            {   
                {
                    mirrorLeft
                },
                {{EYES, Eye::powerstick}}
            },
        },
        "End 4000",
        nullptr
    };

    // TODO: bug in the tweening system causes tweened variables to inhibit previous values for one frame, tween in keyframe 0 is a workaround
    Sequence powerstickVacuumFail {
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