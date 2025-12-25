#ifdef GAMEPAD_XBOX
#pragma once
#include "Typedefs.h"
#include "InputHandler.h"

// input config for Xbox gamepad
namespace Gamepad {
    const char* DEVICE_NAME = "Xbox Wireless Controller";
    const bool reportMapRequired = true;

    //  data[12] - directional pad
    const u8
        XBOX_U = 1,
        XBOX_UR = 2,
        XBOX_R = 3,
        XBOX_DR = 4,
        XBOX_D = 5,
        XBOX_DL = 6,
        XBOX_L = 7,
        XBOX_UL = 8;

    // data[13] - face buttons and bumpers
    const u8
        XBOX_A = 0x01,
        XBOX_B = 0x02,
        XBOX_X = 0x08,
        XBOX_Y = 0x10,
        XBOX_LB = 0x40,
        XBOX_RB = 0x80;

    // data[14] - misc buttons
    const u8
        XBOX_SELECT = 0x04,
        XBOX_START = 0x08,
        XBOX_HOME = 0x10,
        XBOX_LS = 0x20,
        XBOX_RS = 0x40;

    // data[15] - share button
    const u8
        XBOX_SHARE = 0x01;

    u8 prevData[16] = {0};
    // parse hid notifications
    void readInput (
        u16 handle,
        u8* data,
        size_t length,
        InputHandler* inputHandler
    ) {
        if (handle != 30) return;

        if(prevData[1] != data[1] || prevData[3] != data[3]) inputHandler->handleJoystick(0, data[1], data[3]);  // left joystick
        if(prevData[5] != data[5] || prevData[7] != data[7]) inputHandler->handleJoystick(1, data[5], data[7]);  // right joystick

        // face buttons
        if (u8 diff = data[13] ^ prevData[13]) {    // face buttons
            const u8 buttons[6] = {XBOX_A, XBOX_B, XBOX_X, XBOX_Y, XBOX_LB, XBOX_RB};
            const Button mappings[6] = {BTN_A, BTN_B, BTN_X, BTN_Y, BTN_L1, BTN_R1};
            for (size_t i = 0; i < sizeof(buttons); i++) {
                if (diff & buttons[i]) {
                    inputHandler->queueInput({mappings[i], (data[13] & buttons[i]) > 0 ? BTN_STATE_PRESSED : BTN_STATE_RELEASED});
                }
            }
        }

        // misc buttons
        if (u8 diff = data[14] ^ prevData[14]) {    // face buttons
            const u8 buttons[5] = {XBOX_SELECT, XBOX_START, XBOX_HOME, XBOX_LS, XBOX_RS};
            const Button mappings[5] = {BTN_SELECT, BTN_START, BTN_HOME, BTN_L3, BTN_R3};
            for (size_t i = 0; i < sizeof(buttons); i++) {
                if (diff & buttons[i]) {
                    inputHandler->queueInput({mappings[i], (data[14] & buttons[i]) > 0 ? BTN_STATE_PRESSED : BTN_STATE_RELEASED});
                }
            }
        }

        // share button
        if (u8 diff = data[15] ^ prevData[15]) {
            inputHandler->queueInput({BTN_SHARE, data[15] & XBOX_SHARE ? BTN_STATE_PRESSED : BTN_STATE_RELEASED});
        }

        // directional pad
        if (data[12] != prevData[12]) {
            const Button mappings[8] = {BTN_UP, BTN_UP_RIGHT, BTN_RIGHT, BTN_DOWN_RIGHT, BTN_DOWN, BTN_DOWN_LEFT, BTN_LEFT, BTN_UP_LEFT};
            if (data[12] == 0) inputHandler->queueInput({mappings[prevData[12] - 1], BTN_STATE_RELEASED});
            else inputHandler->queueInput({mappings[data[12] - 1], BTN_STATE_PRESSED});
        }

        // todo: analog triggers
        // right trigger
        if (u8 diff = data[11] ^ prevData[11]) {
            if      (data[11] == 3) inputHandler->queueInput({BTN_R2, BTN_STATE_PRESSED});
            else if (data[11] == 0) inputHandler->queueInput({BTN_R2, BTN_STATE_RELEASED});
        }

        // left trigger
        if (u8 diff = data[9] ^ prevData[9]) {
            if      (data[9] == 3) inputHandler->queueInput({BTN_R2, BTN_STATE_PRESSED});
            else if (data[9] == 0) inputHandler->queueInput({BTN_R2, BTN_STATE_RELEASED});
        }

        memcpy(prevData, data, length);
    }
};
#endif