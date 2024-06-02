#include "config.h"
#ifdef GAMEPAD_MOCUTE052F_BLUEDROID
#pragma once
/* This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this software is
   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#include "esp_hidh.h"
#include "esp_hid_gap.h"

#include "Utilities/Typedefs.h"
#include "../Gamepad.h"

#define SCAN_DURATION_SECONDS 5

namespace BLEGamepadExample {
    void hidh_callback(void *handler_args, esp_event_base_t base, i32 id, void *event_data);
    void hid_demo_task(void *pvParameters);
    void create_hid_demo_task();
    void deinit();
    void init(void);
}
#endif