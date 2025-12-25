#pragma once

#include <stdarg.h>
#include "Arduino.h"
#include "Typedefs.h"
#include "System/Utilities/Globals.h"
#include "System/Drawing/DisplayManager.h"
#include "System/Settings/SettingsManager.h"
#include "Controller.h"

/**
 * Settings controller
 *
 * Displays info and provides a simple way to change settings.
 */
class Controller_Settings : public Controller {
  private:
    DisplayManager* displayManager;
    SettingsManager* settingsManager;
    u8 lastSetting;             // last displayed setting
    u8 settingCursor;           // current setting
    const u8 settingCursorWrapValue = 254;  // setting to wrap around to when out of bounds
    
    u8 statusScreenItemY;
    const u8 statusScreenItemYIncrement = 8;
    const u8 statusScreenItemX = 0;

    void initStatusScreen() {
        displayManager->hud->clearBottom();
        displayManager->hud->drawCenteredText(0, 64, 26, "Push up/down for settings");
    }

    void beginStatusScreenDraw() {
        statusScreenItemY = 24;
    }

    void drawStatusScreenItem(const char* fmt, ...) {
        char buf[32];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        displayManager->hud->drawStr(statusScreenItemX, statusScreenItemY, buf);
        statusScreenItemY += statusScreenItemYIncrement;
    }

    bool settingsCursorOutOfBounds() {
        size_t size = settingsManager->getSettings().size();
        if(settingCursor == settingCursorWrapValue - 1) settingCursor = size - 1;
        else if(settingCursor >= size && settingCursor < settingCursorWrapValue) settingCursor = settingCursorWrapValue;
        return settingCursor >= size;
    }

    void addSettingsCursor(i8 value) {
        settingCursor += value;
        drawSetting();
    }

    void adjustSettingValue(i8 value) {
        if(settingsCursorOutOfBounds()) {
            return;
        }
        
        Setting* setting = settingsManager->getSettings()[settingCursor];
        SettingType type = setting->getSettingType();
        if(type == SETTING_TYPE_DEFAULT || type == SETTING_TYPE_DISABLED || setting->isLocked()) return;
        
        setting->adjust(value);
        drawSetting();
    }

    void drawSetting() {
        if(settingsCursorOutOfBounds()) {
            initStatusScreen();
            lastSetting = settingCursor;
            return;
        }

        Setting* setting = settingsManager->getSettings()[settingCursor];

        displayManager->hud->clearBottom();
        displayManager->hud->drawStrX2(1, 30, true, setting->getName());
        displayManager->hud->drawStrX2(0, 45, setting->getDescription());
        
        char buf[32];
        switch(setting->getSettingType()) {
            case SETTING_TYPE_ACTION:
                sprintf(buf, "%14s", lastSetting == settingCursor ? "Done" : "?"); break;
            case SETTING_TYPE_TOGGLE:
                sprintf(buf, "%14s", setting->getValue() ? "On" : "Off"); break;
            case SETTING_TYPE_VALUE:
                sprintf(buf, "%14d", setting->getValue()); break;
            case SETTING_TYPE_PERCENTAGE:
                sprintf(buf, "%13d%%", (u8)(((f32)setting->getValue() / (f32)setting->getMaxValue()) * 100.0f)); break;
            case SETTING_TYPE_DISABLED:
                sprintf(buf, "%14d", "X"); break;
            default:
                sprintf(buf, "%14d", "err"); break;
        }

        displayManager->hud->drawStrX2(0, 64, true, buf);
        lastSetting = settingCursor;

        // max value, box
        if (setting->getMaxValue() > 1) {
            sprintf(buf, "%13d/%3d", setting->getValue(), setting->getMaxValue());
            if(setting->getSettingType() != SETTING_TYPE_PERCENTAGE)displayManager->hud->drawStr(0, 56, buf);
            displayManager->hud->drawFrame(0, 58, 86, 6);
            displayManager->hud->drawBox(2, 60, map(setting->getValue(), 0, setting->getMaxValue(), 0, 82), 2);
        }

        // count
        sprintf(buf, "%d/%d", settingCursor + 1, settingsManager->getSettings().size());
        displayManager->hud->drawStr(0, 56, buf);

        TOASTER_LOG(
            "Setting %d/%d: %s - %s, Value: %d, Min: %d, Max: %d\n", 
            settingCursor + 1, 
            settingsManager->getSettings().size(), 
            setting->getName(),
            setting->getDescription(),
            setting->getValue(), 
            setting->getMinValue(),
            setting->getMaxValue()
        );
    }

  public:
    Controller_Settings(DisplayManager* displayManager, SettingsManager* settingsManager)
        : Controller(CONTROLLER_SETTINGS, SP_NONE)
        , displayManager(displayManager)
        , settingsManager(settingsManager) {
        settingCursor = 255;
        initStatusScreen();
    }

    void update() override {
        if(settingCursor == 255) {
            beginStatusScreenDraw();
            drawStatusScreenItem("BT %22s", btStatus);
            drawStatusScreenItem("Uptime %18s", uptime);
            drawStatusScreenItem("Boops %19d", boopCount);
            drawStatusScreenItem("FPS %21d", (1000000 / deltaTime));
        } else if (settingCursor == 254) { 
            beginStatusScreenDraw();
            drawStatusScreenItem("Frame %19d", frameCount);
            drawStatusScreenItem("FPS %21d", (1000000 / deltaTime));
            drawStatusScreenItem("Heap %20d", esp_get_free_heap_size());
            //drawStatusScreenItem("Mic %21d", micValue);
            drawStatusScreenItem("Version %17s", VERSION_NUMBER);
            drawStatusScreenItem("%25s", COMPILE_TIMESTAMP);
        }
    }

    void handleInput(u8 x, u8 y) override {
        if(y == 0x00) {
            addSettingsCursor(-1);
        } else if(y == 0xFF) {
            addSettingsCursor(1);
        } else if(x == 0x00) {
            adjustSettingValue(-1);
        } else if(x == 0xFF) {
            adjustSettingValue(1);
        }
    }
};