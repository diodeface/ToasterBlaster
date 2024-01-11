#pragma once

#include "Utilities/Typedefs.h"
#include "Arduino.h"
#include "Animation/Bitmaps.h"
#include "Drawing/Effects/Effect.h"
#include "BitmapDraw.h"
#include "config.h"

class Keyframe {
  public:
    std::vector<Effect*> effects;
    std::vector<BitmapDraw> bitmaps;
    i16 next;
    i16 repeat;
    Timestamp duration;

    /**
     * Defines what to do at a given time during a sequence.
     * @param effects vector of Effect objects.
     * @param bitmaps vector of BitmapDraw objects.
     * @param duration Duration of this keyframe.
     * @param next Keyframe index to jump to when this keyframe is over.
     * @param repeat How many times to repeat this keyframe before advancing.
     */
    Keyframe(
        const std::vector<Effect*>& effects = {}, 
        const std::vector<BitmapDraw>& bitmaps = {},
        Timestamp duration = Config::DEFAULT_KEYFRAME_DURATION, 
        i16 next = -1, 
        i16 repeat = 0
    );

    /**
     * Get the first bitmap in this keyframe that corresponds to a bitmask.
     * @param displayBitmask Display bitmask to check.
     * @return Pointer to bitmap if the bitmask is valid, nullptr otherwise.
     */
    Bitmap* getBitmap(DisplayBitmask displayBitmask);

    /**
     * Check if the current keyframe has an effect
     * @param e Effect object to compare
     * @return true if the effect exists in this keyframe, otherwise false
     */
    bool hasEffect(const Effect* e);

    /**
     * Add an effect.
     * @param e Effect object to add
     */
    void addEffect(Effect* e);

    /**
     * Add an effect if this keyframe doesn't contain it yet.
     * @param e Effect object to add
     */
    void addEffectIfNotIncluded(Effect* e);
};

namespace Keyframes {
    extern Keyframe* clearAll;
}