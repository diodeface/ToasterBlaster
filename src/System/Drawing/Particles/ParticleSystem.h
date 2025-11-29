#pragma once

#include <Arduino.h>
#include <vector>
#include "config.h"
#include "Typedefs.h"
#include "Particle.h"
#include "System/Utilities/Util.h"
#include "System/Utilities/Globals.h"

/**
 * Particle system.
 *
 * Each buffer has its own ParticleSystem which can be controlled with effects:
 * EmitParticles, ClearParticles, ParticleColumn, ...
 */
class ParticleSystem {
  private:
    size_t maxParticles;
    bool updated = false;

  public:
    std::vector<Particle> particles;
    /**
     * Construct a ParticleSystem.
     * @param maxParticle Maximum amount of particles.
     */
    ParticleSystem(size_t maxParticles);

    /**
     * Emit particles with given parameters.
     * @param count Amount of particles to emit.
     * @param maxX Maximum X position (width) for particles.
     * @param x X position of the emitter.
     * @param y Y position of the emitter.
     * @param minXSpeed Minimum particle X speed.
     * @param maxXSpeed Maxium particle X speed.
     * @param minYSpeed Minimum particle Y speed.
     * @param maxYSpeed Maxium particle Y speed.
     */
    void emit(
        u8 count, u8 maxX, f32 x, f32 y, f32 minXSpeed, f32 minYSpeed, f32 maxXSpeed, f32 maxYSpeed,
        DrawMode drawMode = DRAW_MODE_1
    );

    void emit(
        u8 count, u8 maxX, f32 x, f32 y, f32 minXSpeed, f32 minYSpeed, f32 maxXSpeed, f32 maxYSpeed, f32 minXGravity,
        f32 minYGravity, f32 maxXGravity, f32 maxYGravity, DrawMode drawMode = DRAW_MODE_1
    );

    /**
     * Emit particles with given parameters.
     * @param count Amount of particles to emit.
     * @param maxX Maximum X position (width) for particles.
     * @param x X position of the emitter.
     * @param y Y position of the emitter.
     * @param minXSpeed Minimum particle X speed.
     * @param maxXSpeed Maxium particle X speed.
     * @param minYSpeed Minimum particle Y speed.
     * @param maxYSpeed Maxium particle Y speed.
     */
    void emit(
        u8 count, u8 maxX, q8_8 x, q8_8 y, q8_8 minXSpeed, q8_8 minYSpeed, q8_8 maxXSpeed, q8_8 maxYSpeed,
        DrawMode drawMode = DRAW_MODE_1
    );

    void emit(
        u8 count, u8 maxX, q8_8 x, q8_8 y, q8_8 minXSpeed, q8_8 minYSpeed, q8_8 maxXSpeed, q8_8 maxYSpeed, q8_8 minXGravity,
        q8_8 minYGravity, q8_8 maxXGravity, q8_8 maxYGravity, DrawMode drawMode = DRAW_MODE_1
    );

    /**
     * Update loop.
     * @return true if particles have been updated, false otherwise.
     */
    bool update();

    /**
     * Clear all particles in this ParticleSystem
     */
    void clear();

    const std::vector<Particle>& getParticles();
};