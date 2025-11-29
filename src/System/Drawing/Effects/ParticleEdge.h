#pragma once

#include "Typedefs.h"
#include "config.h"
#include "Effect.h"
#include "System/Drawing/Particles/ParticleSystem.h"

class ParticleEdge : public Effect {
  public:
    u8 count, side;
    f32 minXSpeed, minYSpeed, maxXSpeed, maxYSpeed;

    /**
     * Finds edges of what's currently displayed from either side and fires particles from that spot.
     * Not applied on repeated redraws.
     * @param displayBitmask Display bitmask
     * @param count Amount of particles to emit on each position.
     * @param side Which side to check from (0 - both, 1 - left, 2 - right)
     * @param minXSpeed Minimum particle X speed.
     * @param maxXSpeed Maxium particle X speed.
     * @param minYSpeed Minimum particle Y speed.
     * @param maxYSpeed Maxium particle Y speed.
     */
    ParticleEdge(
        DisplayBitmask displayBitmask, u8 count, u8 side, f32 minXSpeed, f32 minYSpeed, f32 maxXSpeed, f32 maxYSpeed
    )
        : Effect(displayBitmask, false)
        , count(count)
        , side(side)
        , minXSpeed(minXSpeed)
        , minYSpeed(minYSpeed)
        , maxXSpeed(maxXSpeed)
        , maxYSpeed(maxYSpeed) {
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        size_t size = display->getSize();
        for(u8 y = 0; y < Config::DISPLAY_HEIGHT; y++) {
            // left side
            if(side == 0 || side == 1) {
                for(u8 x = 0; x < size; x++) {
                    if(getPixel(buffer, size, x, y)) {
                        display->particleSystem->emit(count, size, x, y, -maxXSpeed, minYSpeed, -minXSpeed, maxYSpeed);
                        break;
                    }
                }
            }
            // right side
            if(side == 0 || side == 2) {
                for(i8 x = size - 1; x >= 0; x--) {
                    if(getPixel(buffer, size, x, y)) {
                        display->particleSystem->emit(count, size, x, y, minXSpeed, minYSpeed, maxXSpeed, maxYSpeed);
                        break;
                    }
                }
            }
        }
    }
};