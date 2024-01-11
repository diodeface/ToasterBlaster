#include "Keyframe.h"

Keyframe::Keyframe(
    const std::vector<Effect*>& effects, 
    const std::vector<BitmapDraw>& bitmaps,
    Timestamp duration, 
    i16 next, 
    i16 repeat
)
    : effects(effects)
    , bitmaps(bitmaps)
    , duration(duration)
    , next(next)
    , repeat(repeat) {
}

Bitmap* Keyframe::getBitmap(DisplayBitmask displayBitmask) {
    for(const BitmapDraw& b : bitmaps) {
        if(b.displayBitmask & displayBitmask) return b.bitmap;
    }
    return nullptr;
}

bool Keyframe::hasEffect(const Effect* e) {
    for(const Effect* effect : effects) {
        if(e == effect) return true;
    }
    return false;
}

void Keyframe::addEffect(Effect* e) {
    effects.push_back(e);
}

void Keyframe::addEffectIfNotIncluded(Effect* e) {
    if(!hasEffect(e)) {
        effects.push_back(e);
    }
}

namespace Keyframes {
    Keyframe* clearAll = new Keyframe({}, {{Config::Displays::ALL, Bitmaps::Other::empty}});
}