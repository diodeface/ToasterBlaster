#pragma once

#include "Arduino.h"
#include "Typedefs.h"

enum TweenFunction {
    TWEEN_DEFAULT,

    TWEEN_LINEAR,

    TWEEN_IN,
    TWEEN_OUT,
    TWEEN_IN_OUT,

    TWEEN_IN_BACK,
    TWEEN_OUT_BACK,
    TWEEN_IN_OUT_BACK,

    TWEEN_IN_ELASTIC,
    TWEEN_OUT_ELASTIC,
    TWEEN_IN_OUT_ELASTIC,

    TWEEN_IN_BOUNCE,
    TWEEN_OUT_BOUNCE,
    TWEEN_IN_OUT_BOUNCE,

    TWEEN_IN_EXPONENTIAL,
    TWEEN_OUT_EXPONENTIAL,
    TWEEN_IN_OUT_EXPONENTIAL
};

f32 getTweenFunction(TweenFunction tweenFunction, f32 t);