#pragma once

#include "Arduino.h"
#include "Utilities/Typedefs.h"
#include "Utilities/Globals.h"
#include "Controller.h"
#include "Drawing/DisplayManager.h"
#include "Settings/SettingsManager.h"
#include <stdarg.h>

/**
 * Settings controller
 *
 * Displays info and provides a simple way to change settings.
 */
class SettingsController : public Controller {
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

    void addSettingValue(i8 value) {
        if(settingsCursorOutOfBounds()) {
            return;
        }
        settingsManager->getSettings()[settingCursor]->add(value);
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
        displayManager->hud->drawStrX2(0, 32, setting->getName());
        displayManager->hud->drawStrX2(0, 48, setting->getDescription());

        char buf[32];
        switch(setting->getMaxValue()) {
            case 0:
                sprintf(buf, "%13s", lastSetting == settingCursor ? "Done" : "?");
                break;
            case 1:
                sprintf(buf, "%13s", setting->getValue() ? "On" : "Off");
                break;
            default:
                sprintf(buf, "%13d", setting->getValue());
        }
        lastSetting = settingCursor;
        displayManager->hud->drawStrX2(0, 64, buf);
        // max value
        if (setting->getMaxValue() > 1) {
            sprintf(buf, "%13d/%3d", setting->getValue(), setting->getMaxValue());
            displayManager->hud->drawStr(0, 56, buf);
        }
        // count
        sprintf(buf, "%d/%d", settingCursor + 1, settingsManager->getSettings().size());
        displayManager->hud->drawStr(0, 56, buf);
        // bar
        if(setting->getMaxValue() > 0) {
            displayManager->hud->drawFrame(0, 58, 86, 6);
            displayManager->hud->drawBox(2, 60, map(setting->getValue(), 0, setting->getMaxValue(), 0, 82), 2);
        }

        TOASTER_LOG(
            "Setting %d/%d: %s - %s, %d/%d\n", 
            settingCursor + 1, settingsManager->getSettings().size(), 
            setting->getName(), setting->getDescription(),
            setting->getValue(), setting->getMaxValue()
        );
    }

  public:
    SettingsController(DisplayManager* displayManager, SettingsManager* settingsManager)
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
        } else if(x == 0xFF) {
            addSettingValue(-1);
        } else if(x == 0x00) {
            addSettingValue(1);
        }
    }
};