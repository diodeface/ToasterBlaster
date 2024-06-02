#include "q8_8.h"

q8_8 random(q8_8 min, q8_8 max) {
    return q8_8::from_raw_value(random(min.raw_value(), max.raw_value()));
}

q8_8 lerp(q8_8 a, q8_8 b, q8_8 t) {
    return a + t * (b - a);
}