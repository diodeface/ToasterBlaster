#include "config.h"
#ifdef GAMEPAD_MOCUTE052F_BLUEDROID
#pragma once
#include "Utilities/Typedefs.h"
#include "BLEGamepadExample/controller.h"
#include "Peripherals/Gamepad/InputHandler.h"
#include "main.h"

namespace Gamepad {
    void init();
    void update();
    void onStartScan(u8 retryCount);
    void onStopScan(u8 retryCount);
    void onBluetoothInput(esp_hidh_event_data_t* event);
    void onBluetoothConnect(const char* deviceName);
    void onBluetoothDisconnect();
    void onBluetoothBatteryEvent(u8 level);
} 

#endif