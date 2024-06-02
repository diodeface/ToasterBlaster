#pragma once
#include "Utilities/Typedefs.h"
#include "Utilities/Util.h"
#include "Utilities/Globals.h"

static u8 roundAnalogValue(u8 value, u8 threshold = 64) {
    return value > 0x80 + threshold ? 0xff 
        : value < 0x80 - threshold ? 0x00 
        : 0x80;
}

class ButtonDebounce {
  private:
    Timestamp nextActivation, initialDebounceTime, debounceTime;
    u8 activationThreshold;

    void init() {
        debounceTime = initialDebounceTime;
        nextActivation = 0;
    }
  
  public:
    ButtonDebounce(Timestamp initialDebounceTime = 400 MILLIS) : initialDebounceTime(initialDebounceTime) {
        init();
    }

    void reset() {
        init();
    }

    bool shouldActivate() {
        if (micros() > nextActivation) {
            nextActivation = micros() + debounceTime;
            if (debounceTime > 100 MILLIS) {
                debounceTime *= 0.8;
            }
            return true;
        }
        return false;
    }
};