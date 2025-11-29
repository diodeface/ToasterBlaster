#include "config.h"
#ifdef GAMEPAD_MOCUTE052F_NIMBLE
#pragma once
#include <NimBLEDevice.h>
#include "../InputHandler.h"
#include "main.h"
#include "System/Utilities/Globals.h"

/**
 * MOCUTE 052F Gamepad driver using NimBLE-Arduino library.
 */

namespace Gamepad {
    enum GamepadState {
        GAMEPAD_DEFAULT,
        GAMEPAD_DISCONNECTED,
        GAMEPAD_SCANNING,
        GAMEPAD_CONNECTING,
        GAMEPAD_CONNECTED,
    } state;

    // rough key map
    // data[0] joyX
    // data[1] joyY
    // data[7]
    #define GAME_TRIGGER 0x01
    #define GAME_SELECT 0x02
    #define GAME_X 0x08
    #define GAME_Y 0x10
    #define GAME_B 0x80
    // data[8]
    #define GAME_START 0x04
    #define GAME_A 0x08

    u8 data7, data8;
    u8 joyX = 0x80, joyY = 0x80;
    u8 retryCount = 0;
    Timestamp indicatorTime = 0;

    class ToasterBLEClientCallbacks : public NimBLEClientCallbacks {
      public:
        ToasterBLEClientCallbacks (){};

        void onConnect (NimBLEClient* client) override {
            TOASTER_LOG("Bluetooth controller connected %s\n", btStatus);
        }

        void onDisconnect (NimBLEClient* client) override {
            TOASTER_LOG("Bluetooth controller disconnected\n");
            NimBLEDevice::deleteClient(client);
            state = GAMEPAD_DISCONNECTED;
        }

        bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) {
            return false;
        }

        uint32_t onPassKeyRequest() override {
            return 0;
        }

        void onAuthenticationComplete(ble_gap_conn_desc* desc) override {}

        virtual bool onConfirmPIN(uint32_t pin) {
            return true;
        }
    };

    const char* HID_DEVICE_NAME = "MOCUTE-052Fe-AUTO";
    const char* HID_SERVICE = "1812";
    const char* HID_REPORT_DATA = "2A4D";
    NimBLEClient* client;
    NimBLEAddress address;
    NimBLEScanResults results;

    void handleInput (
        NimBLERemoteCharacteristic* characteristic, 
        uint8_t* data, size_t length, bool isNotify) {

        if (characteristic->getHandle() != 56) return;

        joyX = data[0];
        joyY = data[1];

        if (u8 diff = data[7] ^ data7) {
            const u8 buttons[5] = {GAME_TRIGGER, GAME_SELECT, GAME_X, GAME_Y, GAME_B};
            const Button mappings[5] = {BTN_TRIG1, BTN_SELECT, BTN_X, BTN_Y, BTN_B};
            for (size_t i = 0; i < sizeof(buttons); i++) {
                if (diff & buttons[i]) {
                    inputHandler.queueInput({mappings[i], (data[7] & buttons[i]) > 0 ? BTN_STATE_PRESSED : BTN_STATE_RELEASED});
                }
            }
            data7 = data[7];
        }

        if (u8 diff = data[8] ^ data8){
            const u8 buttons[2] = {GAME_START, GAME_A};
            const Button mappings[2] = {BTN_START, BTN_A};
            for (size_t i = 0; i < sizeof(buttons); i++) {
                if (diff & buttons[i]) {
                    inputHandler.queueInput({mappings[i], (data[8] & buttons[i]) > 0 ? BTN_STATE_PRESSED : BTN_STATE_RELEASED});
                } 
            }
            data8 = data[8];
        }

        BT_LOG("BT data: ");
        for (size_t i = 0; i < length; i++) {
            BT_LOG("%02x ", data[i]);
        }
        BT_LOG("\n");
    }

    NimBLEScanResults* startScan() {
        state = GAMEPAD_SCANNING;
        NimBLEScan* pScan = NimBLEDevice::getScan();
        pScan->clearResults();
        pScan->clearDuplicateCache();
        pScan->setActiveScan(true);
        pScan->setInterval(1000);
        pScan->setWindow(900);
        results = pScan->start(2);
        TOASTER_LOG("BT scan finished\n");
        return &results;
    }

    bool tryConnect (NimBLEScanResults* results) {
        state = GAMEPAD_CONNECTING;
        for(size_t i = 0; i < results->getCount(); i++) {
            NimBLEAdvertisedDevice device = results->getDevice(i);
            BT_LOG("%s\n", device.toString().c_str());

            //if (device.getAddress() == address) {
            if (strcmp(device.getName().c_str(), HID_DEVICE_NAME) == 0){
            //if (device.getServiceUUID().equals(NimBLEUUID("1812"))) {
                client = NimBLEDevice::createClient();
                client->setConnectionParams(6, 6, 0, 400);
                client->setClientCallbacks(new ToasterBLEClientCallbacks());

                if(client->connect(&device)) {
                    NimBLERemoteService* svc = nullptr;
                    svc = client->getService(HID_SERVICE);
                    if (svc) {
                        std::vector<NimBLERemoteCharacteristic*>*charvector;
                        charvector = svc->getCharacteristics(true);
                        for (auto &it : *charvector) {
                            BT_LOG("%s\n", it->getUUID().toString().c_str());
                            if (it->getUUID() == NimBLEUUID(HID_REPORT_DATA)) {
                                // Serial.println(it->toString().c_str());
                                if (it->canNotify()) {
                                    if (!it->subscribe(true, handleInput)) {
                                        TOASTER_LOG("BT characteristic subscribe failed\n");
                                        client->disconnect();
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                } else {
                    // failed to connect
                    TOASTER_LOG("Couldn't connect to BT gamepad\n");
                    return false;
                }

                // device connected
                strncpy(btStatus, device.getName().c_str(), 15);
                retryCount = 0;
                return true;
            }
        }

        // no device found
        return false;
    }

    // async
    void update(void *pvParameters) {
        while(true) {
            if(state == GAMEPAD_DISCONNECTED || state == GAMEPAD_DEFAULT) {
                TOASTER_LOG("Scanning for BT Gamepad (Attempt %d)\n", retryCount);
                sprintf(btStatus, "Scanning (%d)", retryCount);
                NimBLEScanResults* results = startScan();
                ++retryCount;
                state = tryConnect(results) ? GAMEPAD_CONNECTED : GAMEPAD_DEFAULT;
            } 
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        vTaskDelete(NULL);
    }

    void update() {
        // Blink LED depending on current BT status
        if (state == GAMEPAD_CONNECTED) {
            digitalWrite(Config::Pins::LED_BUILTIN, LOW);
        } else if (state == GAMEPAD_SCANNING || state == GAMEPAD_CONNECTING) {
            i32 period = state == GAMEPAD_SCANNING ? 500 MILLIS : 250 MILLIS;

            indicatorTime += deltaTime;
            if(indicatorTime > period) {
                digitalWrite(Config::Pins::LED_BUILTIN, HIGH);
            } else {
                digitalWrite(Config::Pins::LED_BUILTIN, LOW);
            }
            if (indicatorTime > period * 2) indicatorTime = 0;
        } else if (state == GAMEPAD_DISCONNECTED) {
            safeMode();
        }

        handleJoystickInput(0xff - joyX, joyY);
    }

    void init() {
        NimBLEDevice::init("");
        pinMode(Config::Pins::LED_BUILTIN, OUTPUT);
        state = GAMEPAD_DEFAULT;
        xTaskCreatePinnedToCore(&update, "gamepadTask", 10*1024, NULL, 2, NULL, 0);
    }

}

#endif