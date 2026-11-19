#pragma once

/**
 * Vacuum cleaner button (pullup) for powerstick synth
 *
 * Custom overrides for sensor events
 */

#include "Sensor.h"

class VacuumCleanerButton : public Sensor {
  private:
    SequencePlayer* sequencePlayer;
  protected:
    void onEnable() override {
        // todo: mp3 player, play windup sound or vacuum sound
        sequencePlayer->transitionSequence(&Sequences::powerstickVacuuming);
    }

    void onDisable() override {
        // play poweroff sound
        sequencePlayer->transitionSequence(&Sequences::powerstickDefault);
    }
    
    void onUpdate() override {
    }

  public:
    VacuumCleanerButton(u8 sensorPin, SequencePlayer* sequencePlayer, bool inverted = false, bool enabled = true)
        : Sensor(sensorPin, 1, 4, inverted, enabled)
    {
        pinMode(sensorPin, INPUT_PULLUP);
        TOASTER_LOG("Vacuum cleaner button initialized");
        this->sequencePlayer = sequencePlayer;
    }
};