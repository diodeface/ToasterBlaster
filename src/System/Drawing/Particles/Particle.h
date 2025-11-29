#pragma once

#include <Arduino.h>
#include "Typedefs.h"
#include "System/Utilities/Util.h"
#include "System/Utilities/q8_8.h"

/**
 * Particle object.
 * @param x X position.
 * @param y Y position.
 * @param xSpeed X speed.
 * @param ySpeed Y speed.
 * @param xGravity X gravity.
 * @param yGravity Y gravity.
 * @param maxX Maximum X position (width) for this particle.
 * @param lifespan Particle lifespan in microseconds.
 * @param type Particle type, specifies how the particle is drawn.
 */
struct Particle {
    q8_8 x, y, xSpeed, ySpeed, xGravity, yGravity;
    i32 lifespan;
    u8 maxX;
    DrawMode drawMode;
};