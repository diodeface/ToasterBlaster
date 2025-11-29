#pragma once

#include "Typedefs.h"
#include "Effect.h"
#include "System/Drawing/Particles/ParticleSystem.h"

// todo: Gravity?

class EmitParticles : public Effect {
  public:
    u8 count;
    q8_8 x, y;
    q8_8 minXSpeed, minYSpeed, maxXSpeed, maxYSpeed;
    q8_8 minXGravity, minYGravity, maxXGravity, maxYGravity;
    DrawMode drawMode;

    /**
     * Emit particles with given parameters.
     * Not applied on repeated redraws.
     * @param displayBitmask Display bitmask
     * @param count Amount of particles to emit
     */
    EmitParticles(
        DisplayBitmask displayBitmask, u8 count, 
        f32 x, f32 y, 
        f32 minXSpeed, f32 minYSpeed, f32 maxXSpeed, f32 maxYSpeed,
        DrawMode drawMode = DRAW_MODE_1
    )
        : Effect(displayBitmask, false)
        , count(count)
        , x(q8_8(x))
        , y(q8_8(y))
        , minXSpeed(q8_8(minXSpeed))
        , minYSpeed(q8_8(minYSpeed))
        , maxXSpeed(q8_8(maxXSpeed))
        , maxYSpeed(q8_8(maxYSpeed))
        , drawMode(drawMode)
        , minXGravity(q8_8(0))
        , minYGravity(q8_8(0))
        , maxXGravity(q8_8(0))
        , maxYGravity(q8_8(0)){};

    /**
     * Emit particles with given parameters.
     * Not applied on repeated redraws.
     * @param displayBitmask Display bitmask
     * @param count Amount of particles to emit
     */
    EmitParticles(
        DisplayBitmask displayBitmask, u8 count, 
        f32 x, f32 y, 
        f32 minXSpeed, f32 minYSpeed, f32 maxXSpeed, f32 maxYSpeed, 
        f32 minXGravity, f32 minYGravity, f32 maxXGravity, f32 maxYGravity,
        DrawMode drawMode = DRAW_MODE_1
    )
        : Effect(displayBitmask, false)
        , count(count)
        , x(q8_8(x))
        , y(q8_8(y))
        , minXSpeed(q8_8(minXSpeed))
        , minYSpeed(q8_8(minYSpeed))
        , maxXSpeed(q8_8(maxXSpeed))
        , maxYSpeed(q8_8(maxYSpeed))
        , minXGravity(q8_8(minXGravity))
        , minYGravity(q8_8(minYGravity))
        , maxXGravity(q8_8(maxXGravity))
        , maxYGravity(q8_8(maxYGravity))
        , drawMode(drawMode){};

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        display->particleSystem->emit(count, display->getSize(), x, y, minXSpeed, minYSpeed, maxXSpeed, maxYSpeed, minXGravity, minYGravity, maxXGravity, maxYGravity, drawMode);
    }
};