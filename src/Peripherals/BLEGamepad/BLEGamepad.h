#pragma once
#include "Utilities/Typedefs.h"
#include "BLEGamepadExample/controller.h"
#include "Peripherals/BLEGamepad/InputHandler.h"
#include "main.h"

namespace BLEGamepad {
    extern InputHandler inputHandler;
    
    void init();
    void update();
    void onStartScan(u8 retryCount);
    void onStopScan(u8 retryCount);
    void onBluetoothInput(esp_hidh_event_data_t* event);
    void onBluetoothConnect(const char* deviceName);
    void onBluetoothDisconnect();
    void onBluetoothBatteryEvent(u8 level);
} 