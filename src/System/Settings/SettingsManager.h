#pragma once

#include <Arduino.h>
#include <vector>
#include "Setting.h"
#include "LambdaSetting.h"
#include "ActionSetting.h"
#include "ComponentToggleSetting.h"
#include "SettingKeys.h"

class SettingsManager {
  private:
    std::vector<Setting*> settings;

    u32 getChecksum(bool fromSaved = true) {
        u32 checksum = 0x13371337;
        for(Setting* setting : settings) {
            if(setting->getKey() == SETTING_DEFAULT) continue;
            checksum ^= ((u32)setting->getKey() * (u32)(fromSaved ? getSavedValue(setting->getKey()) : getCurrentValue(setting->getKey())));
        }
        return checksum;
    }

  public:
    SettingsManager() {
        
    }   

    void addSetting(Setting* setting) {
        settings.push_back(setting);
    }

    void addSettings(std::initializer_list<Setting*> settings) {
        this->settings.insert(this->settings.end(), settings);
    }

    bool init() {
        u32 checksum = getChecksum();
        u32 storedChecksum = EEPROM.readUInt(SETTING_COUNT);    // last 4 bytes are reserved for checksum
        if (checksum != storedChecksum) {
            TOASTER_LOG("Settings checksum doesn't match (%08X != %08X). Settings will be restored.\n", checksum, storedChecksum);
            for(Setting* setting : settings) {
                SettingKey key = setting->getKey();
                if (key == SETTING_DEFAULT) continue;
                EEPROM.write(key, setting->getDefaultValue());
            }
            saveSettings();
            TOASTER_LOG("Settings have been restored. Restarting...\n", checksum, storedChecksum);
            return false;
        }
        return true;
    }

    std::vector<Setting*>& getSettings() {
        return settings;
    }

    Setting* getSetting(SettingKey key) {
        for(Setting* setting : settings) {
            if(key == setting->getKey()) return setting;
        }
        TOASTER_LOG("ERROR: Invalid setting with key %d\n", key);
        return nullptr;
    }

    void saveSettings() {
        EEPROM.writeUInt(SETTING_COUNT, getChecksum());
        EEPROM.commit();
    }

    u8 getCurrentValue(SettingKey key) {
        Setting* setting = getSetting(key);
        return setting ? setting->getValue() : 0;
    }

    static u8 getSavedValue(SettingKey key) {
        return EEPROM.read(key);
    }

    void resetSettings() {
        for(Setting* setting : settings) {
            setting->adjust(setting->getDefaultValue());
        }
    }

    void loadSettings() {
        // run callbacks on all settings that save to EEPROM
        for(Setting* setting : settings) {
            if(setting->getKey() == SETTING_DEFAULT) continue;
            setting->adjust(getCurrentValue(setting->getKey()));
        }
    }
};