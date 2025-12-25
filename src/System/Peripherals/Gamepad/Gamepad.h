#ifdef BT_GAMEPAD
#pragma once
#include <NimBLEDevice.h>
#include "System/Utilities/Globals.h"
#include "InputHandler.h"
#include "ButtonDebounce.h"

namespace Gamepad {
    enum GamepadState {
        GAMEPAD_DEFAULT,
        GAMEPAD_DISCONNECTED,
        GAMEPAD_SCANNING,
        GAMEPAD_CONNECTING,
        GAMEPAD_CONNECTED,
    } state;

    InputHandler inputHandler;
    u8 retryCount = 0;
    Timestamp indicatorTime = 0;

    class ToasterBLEClientCallbacks : public NimBLEClientCallbacks {
      public:
        ToasterBLEClientCallbacks (){};

        void onConnect (NimBLEClient* client) override {

        }

        void onDisconnect (NimBLEClient* client, int reason) override {
            TOASTER_LOG("Bluetooth gamepad disconnected (reason: %d).\n", reason);
            NimBLEDevice::deleteClient(client);
            state = GAMEPAD_DISCONNECTED;
        }

        bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) {
            return false;
        }

        void onAuthenticationComplete(NimBLEConnInfo& connInfo) override {
            return;
        }

        virtual bool onConfirmPIN(uint32_t pin) {
            return true;
        }
    };

    const NimBLEUUID BATTERY_SERVICE = NimBLEUUID((u16)0x180f);
    const NimBLEUUID HID_SERVICE = NimBLEUUID((u16)0x1812);
    const NimBLEUUID HID_REPORT_DATA = NimBLEUUID((u16)0x2a4d);
    const NimBLEUUID HID_REPORT_MAP = NimBLEUUID((u16)0x2a4b);
    
    NimBLEClient* client;
    NimBLEAddress address;
    NimBLEScanResults results;

    void handleInput (
        NimBLERemoteCharacteristic* characteristic, 
        uint8_t* data, size_t length, bool isNotify) { 
        u16 handle = characteristic->getHandle();

        readInput(
            characteristic->getHandle(),
            data,
            length,
            &inputHandler
        );
    }

    NimBLEScanResults* startScan() {
        state = GAMEPAD_SCANNING;
        TOASTER_LOG("Bluetooth gamepad scan started.\n");
        NimBLEScan* pScan = NimBLEDevice::getScan();
        pScan->clearResults();
        pScan->setActiveScan(true);
        pScan->setInterval(500);
        pScan->setWindow(400);
        results = pScan->getResults(2000, false);
        TOASTER_LOG("Bluetooth gamepad scan finished.\n");
        return &results;
    }

    void failConnection (const char* message) {
        client->disconnect();
        strncpy(btStatus, "Error", 15);
        TOASTER_LOG(message);
    }

    bool tryConnect (NimBLEScanResults* results) {
        for(size_t i = 0; i < results->getCount(); i++) {
            const NimBLEAdvertisedDevice* device = results->getDevice(i);
            BT_LOG("%s\n", device->toString().c_str());

            if (strcmp(device->getName().c_str(), DEVICE_NAME) == 0){
                strncpy(btStatus, "Connecting 1/3", 15);
                TOASTER_LOG("Found bluetooth gamepad, connecting.\n");
                state = GAMEPAD_CONNECTING;

                client = NimBLEDevice::createClient();
                client->setConnectionParams(6, 6, 0, 400);
                client->setClientCallbacks(new ToasterBLEClientCallbacks());

                if(client->connect(device)) {
                    if (reportMapRequired && !client->secureConnection(false)) {
                        failConnection("Bluetooth secure connection failed.\n");
                        return false;
                    }

                    NimBLERemoteService* svc = nullptr;
                    svc = client->getService(HID_SERVICE);
                    if (svc) {
                        auto charvector = svc->getCharacteristics(false);
                        if (charvector.empty()) charvector = svc->getCharacteristics(true);
                        for (auto &it : charvector) {
                            if (!client->isConnected()) {
                                failConnection("Bluetooth gamepad refused connection, try pairing mode.\n");
                                return false;
                            }

                            BT_LOG("%s\n", it->getUUID().toString().c_str());
                            // xbox gamepad requires reading report map before accepting a connection
                            if (reportMapRequired && it->getUUID() == HID_REPORT_MAP && it->canRead()) {
                                strncpy(btStatus, "Connecting 2/3", 15);
                                it->readValue();
                            }

                            // subscribe to HID reports
                            if (it->getUUID() == HID_REPORT_DATA && it->canNotify()) {
                                strncpy(btStatus, "Connecting 3/3", 15);
                                if (!it->subscribe(true, handleInput)) {
                                    failConnection("Bluetooth gamepad HID report data subscribe failed.\n");
                                    return false;
                                }
                            }
                        }
                    }
                } else {
                    // failed to connect
                    failConnection("Bluetooth gamepad connection failed.\n");
                    return false;
                }
                
                // device connected
                strncpy(btStatus, device->getName().c_str(), 15);
                TOASTER_LOG("Bluetooth gamepad connected (%s).\n", device->getName().c_str());
                retryCount = 0;
                return true;
            }
        }

        // no device found
        return false;
    }

    // async
    void asyncUpdate(void *pvParameters) {
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

    void update(std::function<void()> panicFunc) {
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
            panicFunc();
        }

        inputHandler.update();
    }

    void init() {
        NimBLEDevice::init("");
        NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
        NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_RANDOM);
        NimBLEDevice::setSecurityAuth(true, true, true);
        pinMode(Config::Pins::LED_BUILTIN, OUTPUT);
        state = GAMEPAD_DEFAULT;
        xTaskCreatePinnedToCore(&asyncUpdate, "gamepadTask", 10*1024, NULL, 2, NULL, 0);
    }
}
#endif