#pragma once

#include "Utilities/Typedefs.h"
#include "Effect.h"
#include "config.h"

class ClearParticles : public Effect {
  public:
    /**
     * Clears all particles for given displays.
     * Not applied on repeated redraws.
     * @param displayBitmask Display bitmask
     */
    ClearParticles(DisplayBitmask displayBitmask = Config::Displays::ALL)
        : Effect(displayBitmask, false) {
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        display->particleSystem->clear();
    }
};