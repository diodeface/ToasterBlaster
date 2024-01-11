#pragma once

/**
 * Generic sensor
 *
 * Override onEnable, onDisable, onUpdate and other virtual functions
 * to add actions on sensor events.
 */

#include "Utilities/Util.h"
#include "Utilities/Globals.h"
#include "Arduino.h"
#include "Utilities/Typedefs.h"
#include "Components/ToasterComponent.h"

class Sensor : public ToasterComponent {
  protected:
    const u8 sensorPin;
    const u8 triggerCount;  // if the sensor is high for this amount of updates, the onEnable() function is called
    const u8 maxTriggers;   // trigger limit, needs to be higher than triggerCount, adds a small delay on sensor deactivation
    u8 triggers;            // number of triggers, amount goes up when high, down when low

    Timestamp time, period;

    bool inverted = false;

    virtual void onTriggerAmountChange(u8 value) {
        TOASTER_LOG("Sensor triggered! %d/%d\n", value, maxTriggers);
    }

    virtual void onEnable() {
    }

    virtual void onDisable() {
    }
    
    virtual void onUpdate() {
    }

    virtual bool getReadout() {
        return digitalRead(sensorPin);
    }

  public:
    Sensor(u8 sensorPin, u8 triggerCount, u8 maxTriggers, bool inverted = false, bool enabled = true)
        : sensorPin(sensorPin)
        , triggerCount(triggerCount)
        , maxTriggers(maxTriggers)
    {
        this->enabled = enabled;
        this->inverted = inverted;
        pinMode(sensorPin, INPUT);
        triggers = 0;
        time = 0;
        period = 50000;
    }

    void start() override {
        enabled = true;
        time = 0;
    }

    void stop() override {
        onDisable();
        enabled = false;
        time = 0;
    }

    void pause() override {
        enabled = false;
    }

    void reset() override {
        onDisable();
        start();
    }

    void update() override {
        if(enabled) {
            if((time += deltaTime) < period) return;
            time = 0;

            if(getReadout() != inverted) {
                if(triggers > 0) {
                    onTriggerAmountChange(--triggers);
                }
            } else {
                if(triggers < maxTriggers) {
                    onTriggerAmountChange(++triggers);
                }
            }

            if(triggers >= triggerCount) {
                onEnable();
            } else {
                onDisable();
            }
        }
        onUpdate();
    }
};