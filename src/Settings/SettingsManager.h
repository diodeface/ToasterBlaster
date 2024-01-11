#pragma once

#include <Arduino.h>
#include <vector>
#include "Setting.h"
#include "LambdaSetting.h"
#include "ComponentToggleSetting.h"

class SettingsManager {
  private:
    std::vector<Setting*> settings;

  public:
    void addSetting(Setting* setting) {
        settings.push_back(setting);
    }

    void addSettings(std::initializer_list<Setting*> settings) {
        this->settings.insert(this->settings.end(), settings);
    }

    std::vector<Setting*>& getSettings() {
        return settings;
    }
};