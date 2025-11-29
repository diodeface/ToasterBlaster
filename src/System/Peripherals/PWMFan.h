#pragma once

#include <Arduino.h>
#include "Typedefs.h"
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

    void setSpeed(u8 speed) {
        this->speed = speed;
        ledcWrite(channel, speed);
    }

    PWMFan(u8 pin, u8 channel, u8 speed = 255)
        : channel(channel)
        , speed(speed) {
        ledcSetup(channel, Config::FAN_PWM_FREQUENCY, Config::FAN_PWM_RESOLUTION);
        ledcAttachPin(pin, channel);
        setSpeed(speed);
    }
};