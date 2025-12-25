#pragma once

#include "Typedefs.h"
#include "Arduino.h"
#include <vector>

/**
 * Configuration constants for used for setup.
 */

 
// Bluetooth Gamepad selection
// To avoid connection issues, erase flash before changing gamepad type
// (PlatformIO menu -> Project Tasks -> esp32dev -> Platform -> Erase flash)
// If for any reason your bluetooth gamepad name does not match, you can change it in System/Peripherals/Gamepad/InputConfig_xxx

#define GAMEPAD_MOCUTE052F       // mocute 052f gamepad
//#define GAMEPAD_XBOX             // Xbox Wireless gamepad
//#define GAMEPAD_DEBUG            // debug gamepad that prints out HID data


namespace Config {
    // Pin numbers for components
    namespace Pins {
        const u8 
        // Built-in LED
        LED_BUILTIN = 2,
        // Main outer display
        MATRIX_DIN = 23,
        MATRIX_CS = 5,
        MATRIX_CLK = 18,
        // Infrared (boop) sensor
        BOOP_DIGITAL = 19,
        BOOP_ANALOG = 39,
        // Heads up display
        HUD_SCL = 22,
        HUD_SDA = 21,
        // Microphone
        MIC = 36,
        // Fan PWM control
        FAN_PWM = 16;
    }

    namespace Displays {
        const u8 
        NUM_DISPLAYS = 14;     // number of chained MAX7219 devices
        
        const u8 
        POS_EYE_L =     96,    // eye left
        POS_EYE_R =     0,     // eye right
        POS_MOUTH_L =   64,    // mouth left
        POS_MOUTH_R =   16,    // mouth right
        POS_NOSE_L =    56,    // nose left
        POS_NOSE_R =    48;    // nose left

        // Display Bitmasks (identifier)
        // Leave these unless you want to add or remap displays
        // If more than 7 displays are required, change the type of DisplayBitmask in Typedefs.h to u16 or u32
        const DisplayBitmask
        EYE_L =         1,        // eye left
        EYE_R =         1 << 1,   // eye right
        MOUTH_L =       1 << 2,   // mouth left
        MOUTH_R =       1 << 3,   // mouth right
        NOSE_L =        1 << 4,   // nose left
        NOSE_R =        1 << 5,   // nose right
        BITMAPBUFFER =  1 << ((sizeof(DisplayBitmask) * 8) - 1),  // bitmap buffer, reserves the leftmost bit
        // Display Bitmask groups
        DEFAULT_DISPLAY = EYE_L,    // default display, set this to any SINGLE display that's in use as some effects depend on it
        EYES = EYE_L | EYE_R,
        NOSE = NOSE_L | NOSE_R,
        MOUTH = MOUTH_L | MOUTH_R,
        LEFT_SIDE = EYE_L | NOSE_L | MOUTH_L,
        RIGHT_SIDE = EYE_R | NOSE_R | MOUTH_R,
        ALL = EYE_L | EYE_R | NOSE_L | NOSE_R | MOUTH_L | MOUTH_R;
    }

    // Miscellaneous settings, best to leave these as is
    const u16 FPS_LIMIT = 120;  // Framerate is limited to avoid fixed point number inaccuracy related issues
    const Timestamp FRAMETIME = 1000000 / FPS_LIMIT;

    // default duration for keyframes
    const Timestamp 
    DEFAULT_KEYFRAME_DURATION = 500 MILLIS,
    DEFAULT_OVERLAY_KEYFRAME_DURATION = 0;

    const u8 
    BOOP_TRIGGER_COUNT = 4,             // how many updates to wait until boop animation triggers
    BOOP_TRIGGERS_MAX = 6,              // maximum value for boop activation (must be higher than activation threshold)
    FAN_PWM_CHANNEL = 0,
    FAN_PWM_RESOLUTION = 8;
    const u32 FAN_PWM_FREQUENCY = 25000;
    const i32 DEFAULT_PARTICLE_LIFESPAN = 3 SECONDS;

    const u8 DISPLAY_HEIGHT = 8;        // do NOT touch this >:(
}