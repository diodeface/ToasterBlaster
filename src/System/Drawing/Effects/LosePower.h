#pragma once

#include "Typedefs.h"
#include "config.h"
#include "Effect.h"
#include "Clear.h"

class LosePower : public Effect {
  public:
    u8 chance;

    /**
     * Hides a bitmap with random chance
     * @param displayBitmask Display bitmask
     * @param chance Chance out of 100
     */
    LosePower(DisplayBitmask displayBitmask = Config::Displays::ALL, u8 chance = 0)
        : Effect(displayBitmask)
        , chance(chance) {
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        if(random(100) > chance) return;
        Clear::clear(display->hudBuffer, display->getSize());
    }
};