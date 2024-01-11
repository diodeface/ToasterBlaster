#pragma once

/**
 * Boop sensor
 * Overlays a face upon infrared sensor activation
 */

#include "Utilities/Typedefs.h"
#include "Utilities/Globals.h"
#include "Sensor.h"
#include "BoopSensor.h"

class DigitalBoopSensor : public Sensor {
  private:
    BoopSensor* boopSensor;

  protected:
    void onEnable() override {
        boopSensor->onEnable();
    }

    void onDisable() override {
        boopSensor->onDisable();
    }

    void onUpdate() override {
        boopSensor->onUpdate();
    }

  public:
    DigitalBoopSensor(
        DisplayManager* displayManager, 
        OverlayPlayer* overlay, 
        OverlaySequence* enableOverlay,
        OverlaySequence* disableOverlay, 
        u8 sensorPin, 
        u8 triggerCount, 
        u8 maxTriggers,
        bool inverted = false,
        bool enabled = true
    )
        : Sensor(sensorPin, triggerCount, maxTriggers, inverted, enabled)
    {
        boopSensor = new BoopSensor(displayManager, overlay, enableOverlay, disableOverlay);
        TOASTER_LOG(
            "Digital boop sensor initialized with Pin: %d, Trigger count: %d, Enabled: %d\n", sensorPin,
            triggerCount, enabled
        );
    }
};