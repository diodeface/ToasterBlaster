#include "config.h"
#include "Gamepad.h"
#ifdef GAMEPAD_MOCUTE052F_BLUEDROID

// keycodes for "GAME" mode
#define GAME_UP 0x1A
#define GAME_DOWN 0x1B
#define GAME_LEFT 0x04
#define GAME_RIGHT 0x07
#define GAME_SELECT 0x0D
#define GAME_START 0x12
#define GAME_X 0x1C
#define GAME_A 0x0F
#define GAME_Y 0x18
#define GAME_B 0x0E
#define GAME_TRIGGER 0x0B

#define GAME_UP_RELEASE 0x08
#define GAME_DOWN_RELEASE 0x1D
#define GAME_LEFT_RELEASE 0x14
#define GAME_RIGHT_RELEASE 0x06
#define GAME_SELECT_RELEASE 0x11
#define GAME_START_RELEASE 0x0A
#define GAME_X_RELEASE 0x17
#define GAME_A_RELEASE 0x19
#define GAME_Y_RELEASE 0x09
#define GAME_B_RELEASE 0x13
#define GAME_TRIGGER_RELEASE 0x15

// keycodes for "KEY" mode
#define KEY_UP 0xB3
#define KEY_DOWN 0xB4
#define KEY_LEFT 0xB6
#define KEY_RIGHT 0xB5
#define KEY_SELECT 0x40
#define KEY_X 0xCD
#define KEY_A 0xE9
#define KEY_Y 0xE2
#define KEY_B 0xEA
#define KEY_TRIGGER 0x28

u8 previousData[18];
u8 previousKey = 0;

/**
 * Legacy gamepad driver for MOCUTE 052F Gamepad
 */

namespace Gamepad {
    enum BTState {
        BTSTATE_DEFAULT,
        BTSTATE_SCANNING,
        BTSTATE_FINISHED
    };

    Timestamp indicatorTime;
    BTState state;

    void init() {
        pinMode(Config::Pins::LED_BUILTIN, OUTPUT);
        BLEGamepadExample::init();
        TOASTER_LOG("MOCUTE gamepad driver initialized.\n");
    }

    // main input update function, calls functions depending on type of data received
    void update() {
        // Blink LED depending on current BT status
        if (state == BTSTATE_DEFAULT) {
            digitalWrite(Config::Pins::LED_BUILTIN, LOW);
        } else if (state == BTSTATE_SCANNING || state == BTSTATE_FINISHED) {
            i32 period = state == BTSTATE_SCANNING ? 500 MILLIS : 250 MILLIS;

            indicatorTime += deltaTime;
            if(indicatorTime > period) {
                digitalWrite(Config::Pins::LED_BUILTIN, HIGH);
                displayManager->hud->drawStatusText(btStatus);
            } else {
                digitalWrite(Config::Pins::LED_BUILTIN, LOW);
                displayManager->hud->drawStatusText("");
            }
            if (indicatorTime > period * 2) indicatorTime = 0;
        }
    }

    ButtonInput translateKeycode(u8 keycode) {
        switch(keycode) {
            case GAME_UP:                   return {BTN_UP, BTN_STATE_PRESSED};
            case GAME_DOWN:                 return {BTN_DOWN, BTN_STATE_PRESSED};
            case GAME_LEFT:                 return {BTN_LEFT, BTN_STATE_PRESSED};
            case GAME_RIGHT:                return {BTN_RIGHT, BTN_STATE_PRESSED};
            case GAME_SELECT:               return {BTN_SELECT, BTN_STATE_PRESSED};
            case GAME_START:                return {BTN_START, BTN_STATE_PRESSED};
            case GAME_X:                    return {BTN_X, BTN_STATE_PRESSED};
            case GAME_A:                    return {BTN_A, BTN_STATE_PRESSED};
            case GAME_Y:                    return {BTN_Y, BTN_STATE_PRESSED};
            case GAME_B:                    return {BTN_B, BTN_STATE_PRESSED};
            case GAME_TRIGGER:              return {BTN_TRIG1, BTN_STATE_PRESSED};

            case GAME_UP_RELEASE:           return {BTN_UP, BTN_STATE_RELEASED};
            case GAME_DOWN_RELEASE:         return {BTN_DOWN, BTN_STATE_RELEASED};
            case GAME_LEFT_RELEASE:         return {BTN_LEFT, BTN_STATE_RELEASED};
            case GAME_RIGHT_RELEASE:        return {BTN_RIGHT, BTN_STATE_RELEASED};
            case GAME_SELECT_RELEASE:       return {BTN_SELECT, BTN_STATE_RELEASED};
            case GAME_START_RELEASE:        return {BTN_START, BTN_STATE_RELEASED};
            case GAME_X_RELEASE:            return {BTN_X, BTN_STATE_RELEASED};
            case GAME_A_RELEASE:            return {BTN_A, BTN_STATE_RELEASED};
            case GAME_Y_RELEASE:            return {BTN_Y, BTN_STATE_RELEASED};
            case GAME_B_RELEASE:            return {BTN_B, BTN_STATE_RELEASED};
            case GAME_TRIGGER_RELEASE:      return {BTN_TRIG1, BTN_STATE_RELEASED};

            case KEY_UP:                    return {BTN_ALT_UP, BTN_STATE_PRESSED};
            case KEY_DOWN:                  return {BTN_ALT_DOWN, BTN_STATE_PRESSED};
            case KEY_LEFT:                  return {BTN_ALT_LEFT, BTN_STATE_PRESSED};
            case KEY_RIGHT:                 return {BTN_ALT_RIGHT, BTN_STATE_PRESSED};
            case KEY_SELECT:                return {BTN_ALT_SELECT, BTN_STATE_PRESSED};
            case KEY_X:                     return {BTN_ALT_X, BTN_STATE_PRESSED};
            case KEY_A:                     return {BTN_ALT_A, BTN_STATE_PRESSED};
            case KEY_Y:                     return {BTN_ALT_Y, BTN_STATE_PRESSED};
            case KEY_B:                     return {BTN_ALT_B, BTN_STATE_PRESSED};
            case KEY_TRIGGER:               return {BTN_ALT_TRIG1, BTN_STATE_PRESSED};

            default:                        return {BTN_NONE, BTN_STATE_RELEASED};
        }
    }

    void printBluetoothData(const u8* data, size_t length) {
        BT_LOG("Bluetooth input: ");
        for(size_t i = 0; i < length; i++) {
            BT_LOG("%02x ", data[i]);
        }
        BT_LOG("\n");
    }

    // called every time data is recieved from the gamepad
    void onBluetoothInput(esp_hidh_event_data_t* event) {
        u8* data = event->input.data;
        u16 length = event->input.length;

        #ifdef BTDEBUG
        printBluetoothData(data, length);
        #endif

        switch (event->feature.usage) {
            case ESP_HID_USAGE_CCONTROL:
                // "KEY" mode
                if (data[0] == 0 && data[1] == 0) {
                    // key released
                    ButtonInput b = translateKeycode(previousKey);
                    b.state = false;
                    inputHandler.queueInput(b);
                } else {
                    // key pressed                                  
                    inputHandler.queueInput(translateKeycode(data[0]));
                    previousKey = data[0];
                }
                break;


            case ESP_HID_USAGE_MOUSE:
                // todo: Analog joystick mode
                break;


            case ESP_HID_USAGE_KEYBOARD:
                // "GAME" mode
                if (data[0] == 0 && data[1] == 0) return;   // ignore empty inputs

                for(size_t i = 0; i < length; i++) {

                    if (data[i] == KEY_TRIGGER) {
                        // trigger in "KEY" mode uses GAMEPAD usage type for some reason? handle it separately
                        // bug: trigger release in "KEY" mode is not detected
                        inputHandler.queueInput(translateKeycode(data[i]));
                        continue;
                    }

                    // ignore duplicate inputs
                    bool isDuplicate = false;
                    for(size_t j = 0; j < length; j++) {
                        if(previousData[i] == data[i]) {
                            isDuplicate = true;
                            break;
                        } 
                    }
                    if(!isDuplicate) inputHandler.queueInput(translateKeycode(data[i]));   // add new input to queue
                }

                // store input for next duplicate check
                for(size_t i = 0; i < length; i++) {
                    previousData[i] = data[i];
                }


            default: 
                break;
        }
    }

    void onStartScan(u8 retryCount) {
        sprintf(btStatus, "Scan (%d)", retryCount);
        BT_LOG("Scanning for BT Gamepad (Attempt %d)\n", retryCount);
        state = BTSTATE_SCANNING;
    }

    void onStopScan(u8 retryCount) {
        sprintf(btStatus, "Finished (%d)", retryCount);
        BT_LOG("BT scan finished (Attempt %d)\n", retryCount);
        state = BTSTATE_FINISHED;
    }

    void onBluetoothConnect(const char* deviceName) {
        strncpy(btStatus, deviceName, 14);
        BT_LOG("Bluetooth controller connected %s\n", deviceName);
        state = BTSTATE_DEFAULT;
    }

    void onBluetoothDisconnect() {
        strcpy(btStatus, "Disconnected");
        BT_LOG("Bluetooth controller disconnected\n");
        BLEGamepadExample::deinit();
        BLEGamepadExample::create_hid_demo_task();
    }

    void onBluetoothBatteryEvent(u8 level) {
        // snprintf(btStatus, 15, "%d", millis());
        BT_LOG("Bluetooth controller battery level changed (%d)\n", level);
    }
}
#endif