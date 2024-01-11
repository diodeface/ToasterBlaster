#pragma once

#include "Utilities/Typedefs.h"
#include "Setting.h"
#include "Components/ToasterComponent.h"

class ComponentToggleSetting : public Setting {
  private:
    ToasterComponent* component;
    const char *name, *description;

  public:
    ComponentToggleSetting(const char* name, const char* description, ToasterComponent* component, u8 eepromAddress = 255)
        : Setting(1, eepromAddress)
        , component(component)
        , name(name)
        , description(description) {
    }

    const char* getName() override {
        return name;
    }

    const char* getDescription() override {
        return description;
    }

    u8 getValue() override {
        return component->isEnabled();
    }

    virtual void add(i8 value) override {
        if(value == -1) {
            component->stop();
        } else {
            component->start();
        }
        if(getEepromAddress() != 255) EEPROM.write(getEepromAddress(), getValue());
    }
};