#pragma once

#include "Utilities/Typedefs.h"
#include "Effect.h"
#include "Drawing/Particles/ParticleSystem.h"

// todo: Gravity?

class EmitParticles : public Effect {
  public:
    u8 count;
    f32 x, y, minXSpeed, minYSpeed, maxXSpeed, maxYSpeed;

    /**
     * Emit particles with given parameters.
     * Not applied on repeated redraws.
     * @param displayBitmask Display bitmask
     * @param count Amount of particles to emit
     */
    EmitParticles(
        DisplayBitmask displayBitmask, u8 count, f32 x, f32 y, f32 minXSpeed, f32 minYSpeed, f32 maxXSpeed,
        f32 maxYSpeed
    )
        : Effect(displayBitmask, false)
        , count(count)
        , x(x)
        , y(y)
        , minXSpeed(minXSpeed)
        , minYSpeed(minYSpeed)
        , maxXSpeed(maxXSpeed)
        , maxYSpeed(maxYSpeed){};

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        display->particleSystem->emit(count, display->getSize(), x, y, minXSpeed, minYSpeed, maxXSpeed, maxYSpeed);
    }
};