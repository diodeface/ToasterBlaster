#pragma once

#include "Utilities/Typedefs.h"
#include "Setting.h"
#include <functional>

class LambdaSetting : public Setting {
    const char *name, *description;
    std::function<u8()> getFunction;
    std::function<void(i8)> addFunction;

  public:
    LambdaSetting(
        const char* name, const char* description, u8 maxValue, std::function<u8()> getFunction, std::function<void(i8)> addFunction,
        u8 eepromAddress = 0
    )
        : name(name)
        , description(description)
        , Setting(maxValue, eepromAddress)
        , getFunction(getFunction)
        , addFunction(addFunction) {
    }

    const char* getName() override {
        return name;
    }

    const char* getDescription() override {
        return description;
    }

    u8 getValue() override {
        return getFunction();
    }

    void add(i8 value) override {
        addFunction(value);
        if(getEepromAddress() != 255) EEPROM.write(getEepromAddress(), getValue());
    }
};