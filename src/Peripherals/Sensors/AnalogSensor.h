#pragma once

/**
 * Generic analog sensor
 *
 * Allows calibration while the program is running
 * 
 * Override onEnable, onDisable, onUpdate and other virtual functions
 * to add actions on sensor events.
 */

#include "Utilities/Util.h"
#include "Utilities/Globals.h"
#include "Arduino.h"
#include "Utilities/Typedefs.h"
#include "Components/ToasterComponent.h"
#include "Peripherals/Sensors/Sensor.h"

class AnalogSensor : public Sensor {
  protected:
    u32 baseline;               // maximum (disabled) sensor value calculated during calibration
    f32 triggerMultiplier;    // baseline multiplier to guess the trigger value
    u32 triggerValue;           // value at which the sensor triggers (baseline * triggerMultiplier)
    SensorState state;          // current state
    
    // calibration related
    bool previousEnabled;               // stores whether or not to enable the sensor after calibration
    const u16 maxCalibrationReadings;   // how many readings to do before returning to tracking state
    u16 calibrationReadings;            // amount of readings done

    bool getReadout() override {
        u16 val = getSensorValue();
        //TOASTER_LOG("Analog sensor value: %d/%d\n", val, triggerValue);
        onTrack(val);
        return val > triggerValue;
    }

    u16 getSensorValue() {
        return analogRead(sensorPin);
    }

    virtual void onCalibrate() {}

    virtual void onTrack(u16 value) {}

  public:
    AnalogSensor(
        u8 sensorPin, 
        u8 triggerCount, 
        u8 maxTriggers, 
        f32 triggerMultiplier = 0.5f,
        u16 maxCalibrationReadings = 50,
        bool inverted = false,
        bool enabled = true
    )
        : Sensor(sensorPin, triggerCount, maxTriggers, inverted, enabled)
        , maxCalibrationReadings(maxCalibrationReadings)
        , state(SENSOR_TRACKING)
    {
        setTriggerMultiplier(triggerMultiplier);
        calibrate();
    }

    void calibrate() {
        if (state == SENSOR_CALIBRATING) return;
        previousEnabled = enabled;
        calibrationReadings = 0;
        baseline = 0;
        state = SENSOR_CALIBRATING;
        stop();
    }

    void setTriggerMultiplier(f32 multiplier) {
        if (multiplier >= 1.0f) triggerMultiplier = 1.0f;
        else if (multiplier <= 0.0f) triggerMultiplier = 0.0f;
        else triggerMultiplier = multiplier;

        triggerValue = baseline * triggerMultiplier;
        TOASTER_LOG("Set trigger multiplier to %.1f, (%d/%d)\n", triggerMultiplier, triggerValue, baseline);
    }

    void addTriggerMultiplier(i8 value) {
        if (value == -1) {
            setTriggerMultiplier(triggerMultiplier - 0.025f);
        } else {
            setTriggerMultiplier(triggerMultiplier + 0.025f);
        }
    }

    u8 getTriggerMultiplier() {
        return nearbyint(triggerMultiplier*100);
    }

    void update() {
        switch(state) {
            case SENSOR_CALIBRATING: {
                if (calibrationReadings < maxCalibrationReadings) {
                    baseline += getSensorValue();
                    ++calibrationReadings;
                    #ifdef DEBUG
                    u32 mod = maxCalibrationReadings * 0.1;
                    if(calibrationReadings % mod == 0) {
                        TOASTER_LOG("Calibrating analog sensor (%d/%d), average = %d.\n", calibrationReadings, maxCalibrationReadings, baseline/calibrationReadings);
                    }
                    #endif
                    onCalibrate();
                } else {
                    baseline /= calibrationReadings;
                    setTriggerMultiplier(triggerMultiplier);
                    state = SENSOR_TRACKING;
                    if(previousEnabled) start();
                    TOASTER_LOG("Finished analog sensor calibration, trigger value: %d * %.1f = %d.\n", baseline, triggerMultiplier, triggerValue);
                }
            }
            break;
            case SENSOR_TRACKING: {
                Sensor::update();
            }
            break;
        }
    }
};