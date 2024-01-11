#pragma once

enum Button {
    BTN_NONE,

    BTN_UP,
    BTN_DOWN,
    BTN_LEFT,
    BTN_RIGHT,
    BTN_SELECT,
    BTN_START,
    BTN_X,
    BTN_A,
    BTN_Y,
    BTN_B,
    BTN_TRIG1,
    BTN_TRIG2,
    
    BTN_ALT_UP,
    BTN_ALT_DOWN,
    BTN_ALT_LEFT,
    BTN_ALT_RIGHT,
    BTN_ALT_SELECT,
    BTN_ALT_START,
    BTN_ALT_X,
    BTN_ALT_A,
    BTN_ALT_Y,
    BTN_ALT_B,
    BTN_ALT_TRIG1,
    BTN_ALT_TRIG2
};

#define BTN_STATE_PRESSED 255
#define BTN_STATE_RELEASED 0