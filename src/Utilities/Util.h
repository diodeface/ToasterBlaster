#pragma once

/*
 * Utility functions and structs.
 */

#include "Utilities/Typedefs.h"
#include "q8_8.h"
#include "Arduino.h"

#ifdef DEBUG
#define TOASTER_LOG(s, ...) printf(s, ##__VA_ARGS__)
#else
#define TOASTER_LOG
#endif
#ifdef BTDEBUG
#define BT_LOG(s, ...) printf(s, ##__VA_ARGS__)
#else
#define BT_LOG
#endif
#ifdef MICDEBUG
#define MIC_LOG(s, ...) printf(s, ##__VA_ARGS__)
#else
#define MIC_LOG
#endif

struct Point {
    u8 x, y;
};

enum SensorState {
    SENSOR_CALIBRATING,
    SENSOR_TRACKING
};

enum DrawMode { 
    DRAW_MODE_0 = 0, 
    DRAW_MODE_1 = 1, 
    DRAW_MODE_XOR 
};

enum Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// converts x, y coordinate to bit(x) and display row(y)
Point pointToMatrix(u8 x, u8 y);

// converts bit and row to x, y coordinate
Point matrixToPoint(u8 bit, u8 row);

// mirrors bits in an integer
u8 mirrorInt(u8 number);

// sets a bit in an integer
u8 setBit(u8 input, u8 bit, u8 value = 1);

// linear interpolation
f32 lerp(f32 a, f32 b, f32 t);

// Helper functions for effects
DrawMode getPixel(const u8* buffer, size_t size, u8 x, u8 y);

u8 getRow(const u8* buffer, size_t size, u8 row);

u8 getColumn(const u8* buffer, size_t size, u8 col);