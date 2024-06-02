#include "ParticleSystem.h"
#include "config.h"

/**
 * Construct a ParticleSystem.
 * @param maxParticle Maximum amount of particles.
 */
ParticleSystem::ParticleSystem(size_t maxParticles) {
    this->maxParticles = maxParticles;
    particles.reserve(maxParticles);
    TOASTER_LOG("Reserved %d particles.\n", maxParticles);
}

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
 * @param minXSpeed Minimum particle X gravity.
 * @param maxXSpeed Maxium particle X gravity.
 * @param minYSpeed Minimum particle Y gravity.
 * @param maxYSpeed Maxium particle Y gravity.
 */
void ParticleSystem::emit(
    u8 count, u8 maxX, q8_8 x, q8_8 y, q8_8 minXSpeed, q8_8 minYSpeed, q8_8 maxXSpeed, q8_8 maxYSpeed, q8_8 minXGravity,
    q8_8 minYGravity, q8_8 maxXGravity, q8_8 maxYGravity, DrawMode drawMode
) {
    for(size_t i = 0; i < count; i++) {
        if(particles.size() >= maxParticles) return;
        particles.push_back(
            {x, y, random(minXSpeed, maxXSpeed), random(minYSpeed, maxYSpeed),
             random(minXGravity, maxXGravity), random(minYGravity, maxYGravity), 
             Config::DEFAULT_PARTICLE_LIFESPAN, maxX, drawMode}
        );
    }
}

/**
 * Emit particles with given parameters.
 * @param count Amount of particles to emit.
 * @param maxX Maximum X position (width) for particles.
 * @param x X position of the emitter.
 * @param y Y position of the emitter.
 * @param minXSpeed Minimum particle X speed.
 * @param minYSpeed Minimum particle Y speed.
 * @param maxXSpeed Maxium particle X speed.
 * @param maxYSpeed Maxium particle Y speed.
 */
void ParticleSystem::emit(
    u8 count, u8 maxX, q8_8 x, q8_8 y, q8_8 minXSpeed, q8_8 minYSpeed, q8_8 maxXSpeed, q8_8 maxYSpeed, DrawMode drawMode
) {
    emit(count, maxX, x, y, minXSpeed, minYSpeed, maxXSpeed, maxYSpeed, q8_8(0), q8_8(0), q8_8(0), q8_8(0), drawMode);
}

void ParticleSystem::emit(
    u8 count, u8 maxX, f32 x, f32 y, f32 minXSpeed, f32 minYSpeed, f32 maxXSpeed, f32 maxYSpeed, DrawMode drawMode
) {
    emit(count, maxX, q8_8(x), q8_8(y), q8_8(minXSpeed), q8_8(minYSpeed), q8_8(maxXSpeed), q8_8(maxYSpeed), drawMode);
}

void ParticleSystem::emit(
    u8 count, u8 maxX, f32 x, f32 y, f32 minXSpeed, f32 minYSpeed, f32 maxXSpeed, f32 maxYSpeed, f32 minXGravity,
    f32 minYGravity, f32 maxXGravity, f32 maxYGravity, DrawMode drawMode
) {
    emit(
        count, maxX, 
        q8_8(x), q8_8(y), q8_8(minXSpeed), q8_8(minYSpeed), q8_8(maxXSpeed), q8_8(maxYSpeed),
        q8_8(minXGravity), q8_8(minYGravity), q8_8(maxXGravity), q8_8(maxYGravity), drawMode
    );
}

/**
 * Update loop.
 * @return true if particles have been updated, false otherwise.
 */
bool ParticleSystem::update() {
    if(particles.empty() && !updated) {
        return false;
    }

    q8_8 delta = q8_8(deltaTime * 0.000001);

    for(i32 i = particles.size() - 1; i >= 0; i--) {
        particles[i].xSpeed += particles[i].xGravity * delta;
        particles[i].ySpeed += particles[i].yGravity * delta;
        particles[i].x += particles[i].xSpeed * delta;
        particles[i].y += particles[i].ySpeed * delta;
        if(particles[i].lifespan <= 0 || particles[i].y >= q8_8(8) || particles[i].y < q8_8(0) ||
           particles[i].x >= q8_8(particles[i].maxX) || particles[i].x < q8_8(0)) {
            particles.erase(particles.begin() + i);
            continue;
        }
        particles[i].lifespan -= deltaTime;
    }
    updated = false;
    return true;
}

/**
 * Clear all particles in this ParticleSystem
 */
void ParticleSystem::clear() {
    particles.clear();
    updated = true;
}

const std::vector<Particle>& ParticleSystem::getParticles() {
    return particles;
}