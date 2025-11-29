#pragma once

#include "Typedefs.h"
#include "Setting.h"
#include "System/Components/ToasterComponent.h"

class ComponentToggleSetting : public Setting {
  private:
    ToasterComponent* component;

  public:
    ComponentToggleSetting(const char* name, const char* description,  SettingKey key, u8 defaultValue, ToasterComponent* component)
        : Setting(name, description, key, SETTING_TYPE_TOGGLE, defaultValue, 0, 1, 1)
        , component(component) 
    {}

    u8 getValue() override {
        if(component == nullptr) return 0;
        return component->isEnabled();
    }

    void adjust(i8 value) override {
        bool result = value > 0;
        result ? component->start() : component->stop();
        if(getSettingType() != SETTING_TYPE_DEFAULT) EEPROM.write(getKey(), result);
    }
};