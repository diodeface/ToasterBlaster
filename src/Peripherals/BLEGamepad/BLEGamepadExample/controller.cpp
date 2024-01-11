/* This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this software is
   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
 */

#include "controller.h"
#include "config.h"

namespace BLEGamepadExample {
    esp_hidh_config_t config = {
        .callback = hidh_callback,
        .event_stack_size = 4096,
        .callback_arg = NULL,
    };

    esp_hidh_dev_t* connected_device;

    void hidh_callback(void *handler_args, esp_event_base_t base, i32 id, void *event_data){
        esp_hidh_event_t event = (esp_hidh_event_t)id;
        esp_hidh_event_data_t *param = (esp_hidh_event_data_t *)event_data;

        switch (event) {
        case ESP_HIDH_OPEN_EVENT: {
            if (param->open.status == ESP_OK) {
                const u8 *bda = esp_hidh_dev_bda_get(param->open.dev);
                ESP_LOGI(TAG, ESP_BD_ADDR_STR " OPEN: %s", ESP_BD_ADDR_HEX(bda), esp_hidh_dev_name_get(param->open.dev));
                esp_hidh_dev_dump(param->open.dev, stdout);
                BLEGamepad::onBluetoothConnect(esp_hidh_dev_name_get(param->open.dev));
            } else {
                ESP_LOGE(TAG, " OPEN failed!");
            }
            break;
        }
        case ESP_HIDH_BATTERY_EVENT: {
            const u8 *bda = esp_hidh_dev_bda_get(param->battery.dev);
            ESP_LOGI(TAG, ESP_BD_ADDR_STR " BATTERY: %d%%", ESP_BD_ADDR_HEX(bda), param->battery.level);
            BLEGamepad::onBluetoothBatteryEvent(param->battery.level);
            break;
        }
        case ESP_HIDH_INPUT_EVENT: {
            const u8 *bda = esp_hidh_dev_bda_get(param->input.dev);
            ESP_LOGI(TAG, ESP_BD_ADDR_STR " INPUT: %8s, MAP: %2u, ID: %3u, Len: %d, Data:", ESP_BD_ADDR_HEX(bda), esp_hid_usage_str(param->input.usage), param->input.map_index, param->input.report_id, param->input.length);
            ESP_LOG_BUFFER_HEX(TAG, param->input.data, param->input.length);
            BLEGamepad::onBluetoothInput(param);
            break;
        }
        case ESP_HIDH_FEATURE_EVENT: {
            const u8 *bda = esp_hidh_dev_bda_get(param->feature.dev);
            ESP_LOGI(TAG, ESP_BD_ADDR_STR " FEATURE: %8s, MAP: %2u, ID: %3u, Len: %d", ESP_BD_ADDR_HEX(bda),
                     esp_hid_usage_str(param->feature.usage), param->feature.map_index, param->feature.report_id,
                     param->feature.length);
            ESP_LOG_BUFFER_HEX(TAG, param->feature.data, param->feature.length);
            break;
        }
        case ESP_HIDH_CLOSE_EVENT: {
            const u8 *bda = esp_hidh_dev_bda_get(param->close.dev);
            ESP_LOGI(TAG, ESP_BD_ADDR_STR " CLOSE: %s", ESP_BD_ADDR_HEX(bda), esp_hidh_dev_name_get(param->close.dev));
            BLEGamepad::onBluetoothDisconnect();
            break;
        }
        default:
            ESP_LOGI(TAG, "EVENT: %d", event);
            break;
        }
    }

    void hid_demo_task(void *pvParameters){
        size_t results_len = 0;
        u8 retry_count = 0;
        while (!connected_device) {
            BLEGamepad::onStartScan(retry_count);
            esp_hid_scan_result_t *results = NULL;
            ESP_LOGI(TAG, "SCAN...");
            esp_hid_scan(SCAN_DURATION_SECONDS, &results_len, &results);
            ESP_LOGI(TAG, "SCAN: %u results", results_len);
            BLEGamepad::onStopScan(retry_count++);
            if (results_len) {
                esp_hid_scan_result_t *r = results;
                esp_hid_scan_result_t *cr = NULL;

                while (r) {   
                    /*
                    printf("  %s: " ESP_BD_ADDR_STR ", ", (r->transport == ESP_HID_TRANSPORT_BLE) ? "BLE" : "BT ", ESP_BD_ADDR_HEX(r->bda));
                    printf("RSSI: %d, ", r->rssi);
                    printf("USAGE: %s, ", esp_hid_usage_str(r->usage));
                    #if CONFIG_BT_BLE_ENABLED
                    if (r->transport == ESP_HID_TRANSPORT_BLE) {
                        //cr = r;
                        printf("APPEARANCE: 0x%04x, ", r->ble.appearance);
                        printf("ADDR_TYPE: '%s', ", ble_addr_type_str(r->ble.addr_type));
                    }
                    #endif 

                    printf("NAME: %s ", buf ? buf : "");
                    printf("\n");
                    */   

                    char buf[128];
                    strcpy(buf, r->name);

                    if (
                        r->transport == ESP_HID_TRANSPORT_BLE &&
                        strcmp(buf, Config::Bluetooth::DEVICE_NAME) == 0
                    ) {
                        cr = r;
                    }

                    r = r->next;
                }
                if (cr) {
                    //open the last result
                    connected_device = esp_hidh_dev_open(cr->bda, cr->transport, cr->ble.addr_type);
                }

                //free the results
                esp_hid_scan_results_free(results);
            }
        }
        vTaskDelete(NULL);
    }

    void create_hid_demo_task(){
        xTaskCreatePinnedToCore(&hid_demo_task, "hid_task", 6 * 1024, NULL, 2, NULL, 1);
    }

    void deinit(){
        // todo: memory not being freed on gamepad disconnect?
        esp_hidh_dev_close(connected_device);
        connected_device = nullptr;
    }

    void init(void){
        btStarted();
        connected_device = nullptr;
        esp_err_t ret;
        #if HID_HOST_MODE == HIDH_IDLE_MODE
        ESP_LOGE(TAG, "Please turn on BT HID host or BLE!");
        return;
        #endif
        ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK( ret );
        ESP_LOGI(TAG, "setting hid gap, mode:%d", HID_HOST_MODE);
        ESP_ERROR_CHECK( esp_hid_gap_init(HID_HOST_MODE) );
        #if CONFIG_BT_BLE_ENABLED
        ESP_ERROR_CHECK( esp_ble_gattc_register_callback(esp_hidh_gattc_event_handler) );
        #endif /* CONFIG_BT_BLE_ENABLED */
        
        ESP_ERROR_CHECK( esp_hidh_init(&config) );

        create_hid_demo_task();
    }
}