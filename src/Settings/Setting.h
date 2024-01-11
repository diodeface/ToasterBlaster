#pragma once

#include "Utilities/Typedefs.h"
#include "EEPROM.h"

class Setting {
  private:
    u8 maxValue, eepromAddress;

  public:
    Setting(
        u8 maxValue = 1,        // 0 for action, 1 for toggle, anything else for adjustable
        u8 eepromAddress = 255  // 255 to disable storing in EEPROM
    )
        : maxValue(maxValue)
        , eepromAddress(eepromAddress) {
    }

    u8 getMaxValue() {
        return maxValue;
    }

    u8 getEepromAddress() {
        return eepromAddress;
    }

    virtual const char* getName() = 0;
    virtual const char* getDescription() = 0;
    virtual u8 getValue() = 0;
    virtual void add(i8 value) = 0;
};