#include "ITweenable.h"

ITweenable::ITweenable(
    f32 start, f32 end,
    Timestamp duration, 
    TweenFunction tweenFunction, 
    bool interruptable,
    bool rounding
)
    : start(start)
    , end(end)
    , duration(duration)
    , tweenFunction(tweenFunction)
    , interruptable(interruptable)
    , rounding(rounding)
    , timer(0.0f) {
}

bool ITweenable::apply() {
    return true;
}

void ITweenable::finish() {
    
}