#pragma once

#include "Arduino.h"
#include "Utilities/Typedefs.h"
#include "Utilities/Globals.h"
#include "Controller.h"
#include "Drawing/DisplayManager.h"
#include <functional>

/**
 * Startup controller
 *
 * Draws a startup screen before running a callback.
 */
class StartupController : public Controller {
  private:
    DisplayManager* displayManager;
    std::function<void()> finishFunction;
    Timestamp time, period;

    void drawLogo(u8 x, u8 y) {
        displayManager->hud->drawFrame(x, y + 9, 38, 10);
        displayManager->hud->drawBox(x, y + 18, 38, 10);
        displayManager->hud->setDrawColor(0);
        displayManager->hud->drawStr(x + 2, y + 26, "BLASTER");
        displayManager->hud->setDrawColor(1);
        displayManager->hud->drawStr(x + 2, y + 17, "Toaster");
        //displayManager->hud->drawStr(x + 2, y + 7, "Adeon's");
        displayManager->hud->drawStrX2(x + 40, y + 28, Config::VERSION_NUMBER);
    }

  public:
    StartupController(DisplayManager* displayManager, Timestamp period, std::function<void()> finishFunction)
        : Controller(CONTROLLER_DEFAULT, SP_NONE)
        , displayManager(displayManager)
        , finishFunction(finishFunction)
        , time(0)
        , period(period) {
        drawLogo(24, 10);
        displayManager->hud->drawCenteredText(0, 48, 26, "github.com/4deon/");
        displayManager->hud->drawCenteredText(0, 56, 26, "ToasterBlaster");
        displayManager->hud->drawFrame(0, 58, 128, 6);
    }

    void update() override {
        displayManager->hud->drawBox(2, 60, map(time, 0, period, 0, 124), 2);

        if ((time += deltaTime) >= period) {
            time = 0;
            finishFunction();
        }
    }

    void handleInput(u8 x, u8 y) override {
        time = 0;
        finishFunction();
    }
};