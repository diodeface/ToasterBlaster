#pragma once

#include <Arduino.h>
#include "Utilities/Typedefs.h"
#include "config.h"

/**
 * Simple PWM Fan controller.
 */
class PWMFan {
  private:
    u8 channel;
    u8 speed;

  public:
    u8 getSpeed() {
        return speed;
    }

    void changeSpeed(u8 speed) {
        this->speed = speed;
        ledcWrite(channel, speed);
    }

    void addSpeed(i8 speed) {
        i32 newSpeed = this->speed + speed;
        if(newSpeed > 255) changeSpeed(255);
        else if(newSpeed < 0) changeSpeed(0);
        else changeSpeed(newSpeed);
    }

    PWMFan(u8 pin, u8 channel, u8 speed = 255)
        : channel(channel)
        , speed(speed) {
        ledcSetup(channel, Config::FAN_PWM_FREQUENCY, Config::FAN_PWM_RESOLUTION);
        ledcAttachPin(pin, channel);
        changeSpeed(speed);
    }
};