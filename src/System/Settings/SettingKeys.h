#pragma once
#include <assert.h>

// For settings that need to be saved and restored using the EEPROM API, declare unique keys for them here:
enum SettingKey {
    SETTING_DEFAULT,                        // Use SETTING_DEFAULT if you don't need to save this setting.
    SETTING_MATRIX_BRIGHTNESS,
    SETTING_ENABLE_BOOP_SENSOR,
    SETTING_ENABLE_AUTO_BLINK,
    SETTING_FAN_PWM_SPEED,
    SETTING_ENABLE_MATRIX,
    SETTING_ENABLE_LEDSTRIP,
    SETTING_BOOP_TRIGGER_MULTIPLIER,
    SETTING_RARE_TRANSITION_CHANCE,
    SETTING_ENABLE_ANIMATED_MOUTH,
    SETTING_COUNT                           // Settings checksum is stored at address `SETTING_COUNT`
};

static_assert(SETTING_COUNT < 508, "SettingKey count cannot be greater than 508.");