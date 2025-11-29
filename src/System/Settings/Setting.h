#pragma once

#include "EEPROM.h"
#include "Typedefs.h"
#include "SettingKeys.h"

// Affects how the setting is drawn and interacted with in the menu
enum SettingType {
    SETTING_TYPE_DEFAULT,    // No action
    SETTING_TYPE_DISABLED,   // Displayed as disabled, no action
    SETTING_TYPE_ACTION,     // Displayed as ?, calls a function
    SETTING_TYPE_TOGGLE,     // Displayed as on/off toggle, calls a function returning bool
    SETTING_TYPE_VALUE,      // Displayed as a u8 value
    SETTING_TYPE_PERCENTAGE  // Displayed as a percentage
};

class Setting {
  private:
    const char *name;
    const char *description;
    u8 defaultValue, minValue, maxValue, increment;
    bool locked;   // true if this setting is locked by the current profile
    SettingType type;
    SettingKey key;

  public:
    /**
     * Construct a Setting object.
     * @param name Setting name.
     * @param name Setting description.
     * @param key Unique key under which this setting is stored in EEPROM. Use `SETTING_DEFAULT` if you don't need to save the value.
     * @param type Defines how the setting is to be rendered. Described further in the definition for `SettingType`.
     * @param defaultValue Default value for this setting.
     * @param minValue Minimum value for this setting.
     * @param maxValue Maximum value for this setting.
     */
    Setting(
        const char* name,
        const char* description,
        SettingKey key = SETTING_DEFAULT,
        SettingType type = SETTING_TYPE_DISABLED,
        u8 defaultValue = 0,
        u8 minValue = 0,
        u8 maxValue = 255,
        i8 increment = 1
    )
        : name(name)
        , description(description)
        , key(key)
        , type(type)
        , defaultValue(defaultValue)
        , minValue(minValue)
        , maxValue(maxValue) 
        , increment(increment)
        , locked(false)
    {}

    const char* getName() {
        return name;
    }

    const char* getDescription() {
        return description;
    }
    
    u8 getMinValue() {
        return minValue;
    }

    u8 getMaxValue() {
        return maxValue;
    }

    u8 getDefaultValue() {
        return defaultValue;
    }

    u8 getIncrement() {
        return increment;
    }

    bool isLocked() {
        return locked;
    }

    SettingType getSettingType() {
        return type;
    }

    SettingKey getKey() {
        return key;
    }

    // Adds `value` to the setting. If the result exceeds minValue or maxValue, it is set to those.
    virtual void adjust(i8 value) {
        value *= increment;
        u8 current = getValue();
        i16 adjusted = current + value;
        if(adjusted > maxValue) adjusted = maxValue;
        if(adjusted < minValue) adjusted = minValue;
        onChange(adjusted);
        if (key != SETTING_DEFAULT) EEPROM.write(getKey(), adjusted);
    }

    virtual u8 getValue() { return 0; };    // getter for the ACTUAL value that this setting is affecting
    virtual void onChange(u8 value) {};     // callback after a new value has been set
};