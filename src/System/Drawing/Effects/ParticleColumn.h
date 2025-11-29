#pragma once

#include "Typedefs.h"
#include "config.h"
#include "Effect.h"
#include "System/Drawing/Particles/ParticleSystem.h"

class ParticleColumn : public Effect {
  public:
    u8 count, col, value;
    f32 minXSpeed, minYSpeed, maxXSpeed, maxYSpeed;

    /**
     * Emits a 8 pixel column of particles according to the bits in an integer.
     * Not applied on repeated redraws.
     * @param displayBitmask Display bitmask
     * @param count Amount of particles to emit on each position.
     * @param col Column (x position) where particles are emitted
     * @param value Integer used to determine y positions where particles are emitted
     * @param minXSpeed Minimum particle X speed.
     * @param maxXSpeed Maxium particle X speed.
     * @param minYSpeed Minimum particle Y speed.
     * @param maxYSpeed Maxium particle Y speed.
     */
    ParticleColumn(
        DisplayBitmask displayBitmask, u8 count, u8 col, u8 value, f32 minXSpeed, f32 minYSpeed, f32 maxXSpeed,
        f32 maxYSpeed
    )
        : Effect(displayBitmask, false)
        , count(count)
        , col(col)
        , value(value)
        , minXSpeed(minXSpeed)
        , minYSpeed(minYSpeed)
        , maxXSpeed(maxXSpeed)
        , maxYSpeed(maxYSpeed) {
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        size_t size = display->getSize();
        for(size_t i = 0; i < Config::DISPLAY_HEIGHT; i++)
            if(bitRead(value, 7 - i))
                display->particleSystem->emit(count, size, col, i, minXSpeed, minYSpeed, maxXSpeed, maxYSpeed);
    }
};