#pragma once

#include <functional>
#include "Typedefs.h"
#include "Setting.h"

class Setting_Action : public Setting {
    std::function<void(u8)> setFunction;

  public:
    Setting_Action(
        const char* name, 
        const char* description, 
        std::function<void(i8)> setFunction
    )
        : Setting(name, description, SETTING_DEFAULT, SETTING_TYPE_ACTION, 0, 0, 0, 0)
        , setFunction(setFunction) {
    }

    void onChange(u8 value) override {
        setFunction(value);
    }
};