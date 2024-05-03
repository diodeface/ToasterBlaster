#pragma once

#include "Arduino.h"

typedef unsigned char       u8;
typedef signed char         i8;
typedef unsigned short      u16;
typedef signed short        i16;
typedef unsigned int        u32;
typedef signed int          i32;
typedef unsigned long long  u64;
typedef signed long long    i64;
typedef float               f32;
typedef double              f64;

typedef unsigned long       Timestamp;
typedef const u8            Bitmap;
typedef u8                  DisplayBitmask;

#define SECONDS             * 1000000
#define MILLIS              * 1000