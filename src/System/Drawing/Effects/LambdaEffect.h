#pragma once

#include <functional>

#include "Typedefs.h"
#include "Effect.h"
#include "System/Drawing/Particles/ParticleSystem.h"

class LambdaEffect : public Effect {
  public:
    std::function<void(u8*, Display*)> function;

    /**
     * Applies your own lambda function.
     * @param displayBitmask Display bitmask
     * @param function Custom apply function
     * @param applyOnRedraw True if the effect should be applied on each redraw.
     */
    LambdaEffect(DisplayBitmask displayBitmask, std::function<void(u8*, Display*)> function, bool applyOnRedraw = true)
        : Effect(displayBitmask, applyOnRedraw)
        , function(function){};

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        function(buffer, display);
    }
};