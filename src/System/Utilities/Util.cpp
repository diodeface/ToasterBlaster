#include "Util.h"

Point pointToMatrix(u8 x, u8 y) {
    // todo: odd edge cases where x and y is off screen?
    return {(u8)(7 - x % 8), (u8)(y + x / 8 * 8)};
}

Point matrixToPoint(u8 bit, u8 row) {
    u8 offset = row / 8 * 8;
    return {(u8)(7 - bit + offset), (u8)(row - offset)};
}

u8 mirrorInt(u8 number) {
    u8 result = 0;
    for(u8 i = 0; i < 8; i++) {
        result = (result << 1) | (number & 1);
        number >>= 1;
    }
    return result;
}

u8 setBit(u8 input, u8 bit, u8 value) {
    return input |= (value << bit);
}

f32 lerp(f32 a, f32 b, f32 t) {
    return a + t * (b - a);
}

DrawMode getPixel(const u8* buffer, size_t size, u8 x, u8 y) {
    Point p = pointToMatrix(x, y);
    if(p.y > size) return DRAW_MODE_0;
    return (DrawMode)bitRead(buffer[p.y], p.x);
}

u8 getRow(const u8* buffer, size_t size, u8 row) {
    if(row > size) return 0;
    return buffer[row];
}

u8 getColumn(const u8* buffer, size_t size, u8 col) {
    u8 result = 0;
    for(u8 i = 0; i < 8; i++) result |= (getPixel(buffer, size, col, 7-i) << i);
    return result;
}