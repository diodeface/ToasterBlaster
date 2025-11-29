#pragma once

#include <functional>
#include "Typedefs.h"
#include "Setting.h"

class LambdaSetting : public Setting {
    
    std::function<u8()> getFunction;
    std::function<void(u8)> setFunction;

  public:
    LambdaSetting(
        const char* name, 
        const char* description, 
        SettingKey key, 
        SettingType type,
        u8 defaultValue,
        u8 minValue,
        u8 maxValue, 
        u8 increment,
        std::function<u8()> getFunction, 
        std::function<void(u8)> setFunction
    )
        : Setting(name, description, key, type, defaultValue, minValue, maxValue, increment)
        , getFunction(getFunction)
        , setFunction(setFunction) {
    }

    u8 getValue() override {
        return getFunction();
    }

    void onChange(u8 value) override {
        setFunction(value);
    }
};