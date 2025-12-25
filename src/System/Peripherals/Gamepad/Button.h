#pragma once

#include "Typedefs.h"

enum Button {
    BTN_NONE,

    BTN_UP,
    BTN_UP_RIGHT,
    BTN_RIGHT,
    BTN_DOWN_RIGHT,
    BTN_DOWN,
    BTN_DOWN_LEFT,
    BTN_LEFT,
    BTN_UP_LEFT,

    BTN_SELECT,
    BTN_START,
    BTN_HOME,
    BTN_SHARE,

    BTN_X,
    BTN_A,
    BTN_Y,
    BTN_B,

    BTN_L1,
    BTN_R1,
    BTN_L2,
    BTN_R2,
    BTN_L3,
    BTN_R3,
    BTN_L4,
    BTN_R4,
    BTN_L5,
    BTN_R5
};

static const u8 BTN_STATE_PRESSED = 255;
static const u8 BTN_STATE_RELEASED = 0;