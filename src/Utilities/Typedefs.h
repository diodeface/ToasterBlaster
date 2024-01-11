#pragma once

#include "Arduino.h"

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef float_t f32;
typedef double_t f64;

typedef unsigned long Timestamp;
typedef const u8 Bitmap;
typedef u8 DisplayBitmask;

#define SECONDS * 1000000
#define MILLIS * 1000