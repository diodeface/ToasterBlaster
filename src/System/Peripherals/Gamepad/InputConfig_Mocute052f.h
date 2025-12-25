#ifdef GAMEPAD_MOCUTE052F
#pragma once
#include "Typedefs.h"
#include "InputHandler.h"

// input config for MOCUTE 052f gamepad
namespace Gamepad {
    const char* DEVICE_NAME = "MOCUTE-052Fe-AUTO";
    const bool reportMapRequired = false;

    // rough key map
    // GAME mode - handle 56
    // data[0] joyX
    // data[1] joyY
    // data[7]
    const u8 GAME_TRIGGER = 0x01;
    const u8 GAME_SELECT = 0x02;
    const u8 GAME_X = 0x08;
    const u8 GAME_Y = 0x10;
    const u8 GAME_B = 0x80;
    // data[8]
    const u8 GAME_START = 0x04;
    const u8 GAME_A = 0x08;

    // KEY mode - handle 52
    // data[0]
    const u8 KEY_UP = 0xB3;
    const u8 KEY_DOWN = 0xB4;
    const u8 KEY_RIGHT = 0xB5;
    const u8 KEY_LEFT = 0xB6;
    const u8 KEY_X = 0xCD;
    const u8 KEY_Y = 0x40;
    const u8 KEY_B = 0xEA;
    const u8 KEY_A = 0xE9;
    const u8 KEY_SELECT = 0x24;

    // KEY mode - handle 48 (trigger)
    // data[0]
    const u8 KEY_TRIGGER = 0x28;

    // KEY mode - handle 44 (mouse mode)
    // data[0]
    const u8 KEY_MOUSE_TRIGGER = 0x01;
    const u8 KEY_MOUSE_SELECT = 0x02;
    // data[1] joyX in range df - 00 - 1f
    // data[2] joyY in range df - 00 - 1f
    // START button in KEY mode switches between normal and mouse mode
    // KEY mode should only be used as an alternative input, since only one key is sent at a time and because of the strange start button behavior
    
    u8 game_data7, game_data8, key_data0, mouse_data0;
    // parse hid notifications
    void readInput (
        u16 handle,
        u8* data,
        size_t length,
        InputHandler* inputHandler
    ) {
        // "GAME" mode
        if (handle == 56) {
            inputHandler->handleJoystick(0, data[0], data[1]);

            if (u8 diff = data[7] ^ game_data7) {
                const u8 buttons[5] = {GAME_TRIGGER, GAME_SELECT, GAME_X, GAME_Y, GAME_B};
                const Button mappings[5] = {BTN_R1, BTN_SELECT, BTN_X, BTN_Y, BTN_B};
                for (size_t i = 0; i < sizeof(buttons); i++) {
                    if (diff & buttons[i]) {
                        inputHandler->queueInput({mappings[i], (data[7] & buttons[i]) > 0 ? BTN_STATE_PRESSED : BTN_STATE_RELEASED});
                    }
                }
                game_data7 = data[7];
            }

            if (u8 diff = data[8] ^ game_data8){
                const u8 buttons[2] = {GAME_START, GAME_A};
                const Button mappings[2] = {BTN_START, BTN_A};
                for (size_t i = 0; i < sizeof(buttons); i++) {
                    if (diff & buttons[i]) {
                        inputHandler->queueInput({mappings[i], (data[8] & buttons[i]) > 0 ? BTN_STATE_PRESSED : BTN_STATE_RELEASED});
                    } 
                }
                game_data8 = data[8];
            }
        }

        // "KEY" mode
        else if (handle == 52) {
            const u8 buttons[9] = {KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT, KEY_X, KEY_Y, KEY_B, KEY_A, KEY_SELECT};
            const Button mappings[9] = {BTN_UP, BTN_DOWN, BTN_RIGHT, BTN_LEFT, BTN_HOME, BTN_R2, BTN_SHARE, BTN_L2, BTN_L3};    // i am starting to run out of buttons here
            // release previously pressed key
            if (data[0] == 0) {
                for (size_t i = 0; i < sizeof(buttons); i++) {
                    if (key_data0 == buttons[i]) inputHandler->queueInput({mappings[i], BTN_STATE_RELEASED});
                }
            }
            // activate new key
            for (size_t i = 0; i < sizeof(buttons); i++) {
                if (data[0] == buttons[i]) inputHandler->queueInput({mappings[i], BTN_STATE_PRESSED});
            }
            key_data0 = data[0];
        }
        
        // "KEY" mode, trigger
        else if (handle == 48) {
            if (data[0] == KEY_TRIGGER) inputHandler->queueInput({BTN_L1, BTN_STATE_PRESSED});
            if (data[0] == 0x00) inputHandler->queueInput({BTN_L1, BTN_STATE_RELEASED});
        }

        // "KEY" mode, mouse mode
        else if (handle == 44) {
            const u8 buttons[2] = {KEY_MOUSE_TRIGGER, KEY_MOUSE_SELECT};    // KEY_MOUSE_TRIGGER and KEY_MOUSE_SELECT can be treated as separate buttons... to avoid confusion, they will have the same mapping as in dpad mode
            const Button mappings[2] = {BTN_L1, BTN_L3};                    // bug: there seems to be some strange behavior when pressing B in GAME mode, first byte in handle 44 is 1, thus also triggering KEY_MOUSE_TRIGGER
            // release previously pressed key
            if (data[0] == 0) {
                for (size_t i = 0; i < sizeof(buttons); i++) {
                    if (mouse_data0 == buttons[i]) inputHandler->queueInput({mappings[i], BTN_STATE_RELEASED});
                }
            }
            // activate new key
            for (size_t i = 0; i < sizeof(buttons); i++) {
                if (data[0] == buttons[i]) inputHandler->queueInput({mappings[i], BTN_STATE_PRESSED});
            }
            mouse_data0 = data[0];

            // cleaning out cases where 0xdf would cause an undesired overflow
            inputHandler->handleJoystick(
                1, 
                ((data[1] == 0xdf ? 0xe0 : data[1]) + 0x20) << 2, 
                ((data[2] == 0xdf ? 0xe0 : data[2]) + 0x20) << 2
            );
        }
    }
};
#endif