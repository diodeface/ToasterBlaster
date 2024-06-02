#include "Typedefs.h"
#include "fpm/fixed.hpp"

/**
 * Fixed point Q8.8 number format using fpm library
 * Used by particle system in place of floats for better performance
 */

using q8_8 = fpm::fixed<i16, i32, 8>;

extern q8_8 random(q8_8 min, q8_8 max);

extern q8_8 lerp(q8_8 a, q8_8 b, q8_8 t);