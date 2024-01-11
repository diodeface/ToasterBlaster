#pragma once

#include <vector>
#include "Utilities/Typedefs.h"
#include "Effect.h"
#include "Animation/Bitmaps.h"
#include "DrawBitmap.h"

struct SafeBitmapDraw {
    Bitmap* bitmap;
    DisplayBitmask displayBitmask;
    u8 size;
};

class RandomizeFace : public Effect {
  private:
    // we're not using deltaTime here because the apply function might not be called every time
    Timestamp lastTime;
    std::vector<SafeBitmapDraw> bitmapDraws;

    Bitmap* getRandomBitmap(u8 size) {
        return size == 32 ? Bitmaps::mouthBitmaps[random(Bitmaps::mouthBitmaps.size())]
            : size == 16  ? Bitmaps::eyeBitmaps[random(Bitmaps::eyeBitmaps.size())]
                          : Bitmaps::noseBitmaps[random(Bitmaps::noseBitmaps.size())];
    }

    SafeBitmapDraw& getCurrentDraw(Display* display) {
        DisplayBitmask bitmask = display->getBitmask();
        for(SafeBitmapDraw& draw : bitmapDraws) {
            if(draw.displayBitmask & bitmask) {
                return draw;
            }
        }
        u8 size = display->getSize();
        bitmapDraws.push_back({getRandomBitmap(size), bitmask, size});
        return bitmapDraws[bitmapDraws.size() - 1];
    }

  public:
    Timestamp period;

    /**
     * Randomizes face bitmaps in an interval. (work in progress)
     * @param displayBitmask Display bitmask
     * @param period Amount of time between randomizing on redraws
     * ......It's an ugly (but necessary) hack to prevent lots of redraws while overlays are playing
     */
    RandomizeFace(DisplayBitmask displayBitmask, Timestamp period)
        : Effect(displayBitmask)
        , period(period) {
        lastTime = 0;
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);

        if((micros() - lastTime) > period) {
            lastTime = micros();
            for(SafeBitmapDraw& draw : bitmapDraws) {
                draw.bitmap = getRandomBitmap(draw.size);
            }
        }

        SafeBitmapDraw draw = getCurrentDraw(display);
        DrawBitmap::drawBitmap(buffer, draw.size, draw.bitmap, draw.size);
    }
};