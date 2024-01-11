#include "TweenFunction.h"

/**
 * Easing functions ~borrowed~ from https://easings.net/ ;)
 */

const f32 C1 = 1.70158;
const f32 C2 = C1 * 1.525;
const f32 C3 = C1 + 1;
const f32 C4 = (2 * PI) / 3;
const f32 C5 = (2 * PI) / 4.5;


f32 easeOutBounce(f32 t) {
    f32 n1 = 7.5625;
    f32 d1 = 2.75;

    if(t < 1 / d1) {
        return n1 * t * t;
    } else if(t < 2 / d1) {
        return n1 * (t -= 1.5 / d1) * t + 0.75;
    } else if(t < 2.5 / d1) {
        return n1 * (t -= 2.25 / d1) * t + 0.9375;
    } else {
        return n1 * (t -= 2.625 / d1) * t + 0.984375;
    }
}

f32 getTweenFunction(TweenFunction tweenFunction, f32 t) {
    switch(tweenFunction) {
        case TWEEN_LINEAR:
            return t;

        case TWEEN_IN:
            return pow(t, 3);
        case TWEEN_OUT:
            return 1 - pow((1 - t), 3);
        case TWEEN_IN_OUT:
            return (t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2);

        case TWEEN_IN_BACK:
            return C3 * t * t * t - C1 * t * t;
        case TWEEN_OUT_BACK:
            return 1 + C3 * pow(t - 1, 3) + C1 * pow(t - 1, 2);
        case TWEEN_IN_OUT_BACK:
            return t < 0.5 ? (pow(2 * t, 2) * ((C2 + 1) * 2 * t - C2)) / 2
                           : (pow(2 * t - 2, 2) * ((C2 + 1) * (t * 2 - 2) + C2) + 2) / 2;

        case TWEEN_IN_ELASTIC:
            return t == 0 ? 0 : t == 1 ? 1 : -pow(2, 10 * t - 10) * sin((t * 10 - 10.75) * C4);
        case TWEEN_OUT_ELASTIC:
            return t == 0 ? 0 : t == 1 ? 1 : pow(2, -10 * t) * sin((t * 10 - 0.75) * C4) + 1;
        case TWEEN_IN_OUT_ELASTIC:
            return t == 0 ? 0
                : t == 1  ? 1
                : t < 0.5 ? -(pow(2, 20 * t - 10) * sin((20 * t - 11.125) * C5)) / 2
                          : (pow(2, -20 * t + 10) * sin((20 * t - 11.125) * C5)) / 2 + 1;

        case TWEEN_IN_BOUNCE:
            return 1 - easeOutBounce(1 - t);
        case TWEEN_OUT_BOUNCE:
            return easeOutBounce(t);
        case TWEEN_IN_OUT_BOUNCE:
            return t < 0.5 ? (1 - easeOutBounce(1 - 2 * t)) / 2 : (1 + easeOutBounce(2 * t - 1)) / 2;

        case TWEEN_IN_EXPONENTIAL:
            return t == 0 ? 0 : pow(2, 10 * t - 10);
        case TWEEN_OUT_EXPONENTIAL:
            return t == 1 ? 1 : 1 - pow(2, -10 * t);
        case TWEEN_IN_OUT_EXPONENTIAL:
            return t == 0 ? 0 : t == 1 ? 1 : t < 0.5 ? pow(2, 20 * t - 10) / 2 : (2 - pow(2, -20 * t + 10)) / 2;
    }
    return t;
}