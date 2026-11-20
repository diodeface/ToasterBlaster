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
    Rectangle* psRectangle = new Rectangle(EYES, 7, 3, 2, 3, DRAW_MODE_0);

    const Timestamp psRectangleMinWait = 1 SECONDS;
    const Timestamp psRectangleMaxWait = 5 SECONDS;

    void movePsRectangle(Display* display) {
        Point targetPosition = {(u8)random(2, 13), (u8)random(1, 5)};
        Timestamp duration = random(100 MILLIS, 1000 MILLIS);
        display->tweenManager->add(new Tweenable<u8>(&(psRectangle->x), psRectangle->x, targetPosition.x, duration, TWEEN_LINEAR));
        display->tweenManager->add(new Tweenable<u8>(&(psRectangle->y), psRectangle->y, targetPosition.y, duration, TWEEN_LINEAR));
    }

    void crossfadeEffect (u8* buffer, Display* display, Timestamp duration) {{
        u8 size = display->getSize();
        for (u8 x = 0; x < size; x++) {
            for (u8 y = 0; y < Config::DISPLAY_HEIGHT; y++) {
                display->particleSystem->particles.push_back(
                    {q8_8(x), q8_8(y), q8_8(0), q8_8(0), q8_8(0), q8_8(0), (i32)random(duration), size, getPixel(buffer, size, x, y)}
                );
            }
        }
    }}

    const Timestamp startupBlinkTime = 100 MILLIS;
    Sequence powerstickStartup {
        {
            {{ mirrorLeft },{{EYES, Other::empty}}, startupBlinkTime},
            {{ mirrorLeft },{{EYES, Eye::powerstickStartup0}}, startupBlinkTime},
            {{ mirrorLeft },{{EYES, Other::empty}}, startupBlinkTime},
            {{ mirrorLeft },{{EYES, Eye::powerstickStartup0}}, startupBlinkTime},
            {{ mirrorLeft },{{EYES, Other::empty}}, startupBlinkTime},
            {{ mirrorLeft },{{EYES, Eye::powerstickStartup0}}, startupBlinkTime},
            {{ mirrorLeft },{{EYES, Other::empty}}, startupBlinkTime},
            {{ mirrorLeft },{{EYES, Eye::powerstickStartup1}}, 500 MILLIS},
            {{ psRectangle, mirrorLeft, new LambdaEffect(EYES, [](u8* buffer, Display* display) {crossfadeEffect(buffer, display, 1000 MILLIS);}, false) },{{EYES, Eye::powerstickStartup1}}, 1},
            {   
                { psRectangle, mirrorLeft },{{EYES, Eye::powerstick}}, 1000 MILLIS
            },
            {
                { psRectangle, mirrorLeft, new LambdaEffect(DEFAULT_DISPLAY, [](u8* buffer, Display* display) {movePsRectangle(display);}),}, {}, 1
            },
            {   
                { psRectangle, mirrorLeft },{{EYES, Eye::powerstick}}, 1, 10
            }
        },
        "Powerstick!",
        nullptr,
        psRectangleMinWait,
        psRectangleMaxWait
    };

    Sequence powerstickDefault {
        {
            {
                { psRectangle, mirrorLeft, new LambdaEffect(DEFAULT_DISPLAY, [](u8* buffer, Display* display) {movePsRectangle(display);}),}, {}, 1
            },
            {   
                { psRectangle, mirrorLeft },{{EYES, Eye::powerstick}}, 1, 0
            }
        },
        "Powerstick!",
        nullptr,
        psRectangleMinWait,
        psRectangleMaxWait
    };

    Sequence powerstickActivate {
        {
            {
                { psRectangle, mirrorLeft, new LambdaEffect(DEFAULT_DISPLAY, [](u8* buffer, Display* display) {movePsRectangle(display);}),}, {}, 1
            },
            {   
                { psRectangle, mirrorLeft },{{EYES, Eye::powerstick}}, 1, 0
            }
        },
        "Powerstick!",
        &Transitions::glitch,
        psRectangleMinWait,
        psRectangleMaxWait
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
                {}, 1, -2
            }
        },
        "Vacuuming",
        &Transitions::crossfade,
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
                { psRectangle, mirrorLeft, new LambdaEffect(DEFAULT_DISPLAY, [](u8* buffer, Display* display) {movePsRectangle(display);}),}, {}, 1
            },
            {   
                { psRectangle, mirrorLeft },{{EYES, Eye::powerstick}}, 1, 1
            }
        },
        "End 1000",
        nullptr,
        psRectangleMinWait,
        psRectangleMaxWait
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
                { psRectangle, mirrorLeft, new LambdaEffect(DEFAULT_DISPLAY, [](u8* buffer, Display* display) {movePsRectangle(display);}),}, {}, 1
            },
            {   
                { psRectangle, mirrorLeft },{{EYES, Eye::powerstick}}, 1, 1
            }
        },
        "End 1500",
        nullptr,
        psRectangleMinWait,
        psRectangleMaxWait
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
                { psRectangle, mirrorLeft, new LambdaEffect(DEFAULT_DISPLAY, [](u8* buffer, Display* display) {movePsRectangle(display);}),}, {}, 1
            },
            {   
                { psRectangle, mirrorLeft },{{EYES, Eye::powerstick}}, 1, 1
            }
        },
        "End 2000",
        nullptr,
        psRectangleMinWait,
        psRectangleMaxWait
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
                { psRectangle, mirrorLeft, new LambdaEffect(DEFAULT_DISPLAY, [](u8* buffer, Display* display) {movePsRectangle(display);}),}, {}, 1
            },
            {   
                { psRectangle, mirrorLeft },{{EYES, Eye::powerstick}}, 1, 1
            }
        },
        "End 4000",
        nullptr,
        psRectangleMinWait,
        psRectangleMaxWait
    };

    // TODO: bug in the tweening system causes tweened variables to inhibit previous values for one frame, tween in keyframe 0 is a workaround
    Sequence powerstickVacuumFail {
        {
            {{new Tween<u8>(&(psExpandEyes->iterations), 0, 0, 0 MILLIS, TWEEN_IN)},{{EYES, Eye::exclamationPoint}}, 114 MILLIS},
            {{},{{EYES, Other::empty}}, 35 MILLIS},
            {{},{{EYES, Eye::exclamationPoint}}, 184 MILLIS},
            {{},{{EYES, Other::empty}}, 32 MILLIS},
            {{},{{EYES, Eye::exclamationPoint}}, 2000 MILLIS},
            {{psExpandEyes, psRectangle, mirrorLeft, new Tween<u8>(&(psExpandEyes->iterations), 0, 5, 250 MILLIS, TWEEN_LINEAR)},{{}}, 250 MILLIS},
            {
                { psRectangle, mirrorLeft, new LambdaEffect(DEFAULT_DISPLAY, [](u8* buffer, Display* display) {movePsRectangle(display);})}, {{EYES, Eye::powerstick}}, 1
            },
            {   
                { psRectangle, mirrorLeft },{{EYES, Eye::powerstick}}, 1, 6
            }
        },
        "Error",
        nullptr,
        psRectangleMinWait,
        psRectangleMaxWait
    };
}