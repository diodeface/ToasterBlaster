#pragma once

#include <functional>
#include "Arduino.h"
#include "Typedefs.h"

#include "System/Utilities/Globals.h"
#include "System/Drawing/DisplayManager.h"
#include "Assets/Bitmaps/System.h"
#include "Assets/Bitmaps/Standard.h"

#include "Controller.h"

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
    Timestamp timeFace, periodFace;

    void drawLogo() {
        displayManager->hud->setDrawBuffer(HUD_BUFFER_SUB);
        u8 prevDrawColor = displayManager->hud->getDrawColor();
        displayManager->hud->setDrawColor(1);
        displayManager->hud->drawBox(0, 0, 128, 64);
        displayManager->hud->drawXBM(
            4, 5,
            Bitmaps::System::splash1Width,
            Bitmaps::System::splash1Height,
            Bitmaps::System::splash1
        );
        
        displayManager->hud->drawXBM(
            92, 6,
            Bitmaps::System::splash2Width,
            Bitmaps::System::splash2Height,
            Bitmaps::System::splash2
        );
        
        char buf[8];
        snprintf(buf, 8, "%7s", VERSION_NUMBER);
        displayManager->hud->setDrawColor(0);
        displayManager->hud->drawStr(92, 63, buf);
        displayManager->hud->setDrawColor(prevDrawColor);
        displayManager->hud->setDrawBuffer(HUD_BUFFER_MAIN);
    }

    void drawFace() {
        displayManager->hud->setDrawBuffer(HUD_BUFFER_SUB);
        Bitmap* eye = Bitmaps::EyeBitmaps[random(Bitmaps::EyeBitmaps.size())];
        Bitmap* nose = Bitmaps::NoseBitmaps[random(Bitmaps::NoseBitmaps.size())];
        Bitmap* mouth = Bitmaps::MouthBitmaps[random(Bitmaps::MouthBitmaps.size())];
        displayManager->hud->drawBitmap(eye, 16, 48, 35);
        displayManager->hud->drawBitmap(nose, 8, 77, 35);
        displayManager->hud->drawBitmap(mouth, 32, 51, 46);
        displayManager->hud->setDrawBuffer(HUD_BUFFER_MAIN);
    }

    void drawSpinner() {
        f32 t = (float)time / 100000.0f;
        displayManager->hud->setDrawBuffer(HUD_BUFFER_SUB);
        for (u8 i = 1; i < 4; i++) {
            f32 offset = t + (float)i;
            displayManager->hud->drawBox(
                110 - sin(offset) * 5 - (i >> 1),
                40  + cos(offset) * 5 - (i >> 1),
                i, i
            );
        }
        displayManager->hud->setDrawBuffer(HUD_BUFFER_MAIN);
    }

  public:
    StartupController(DisplayManager* displayManager, Timestamp period, std::function<void()> finishFunction)
        : Controller(CONTROLLER_DEFAULT, SP_NONE)
        , displayManager(displayManager)
        , finishFunction(finishFunction)
        , time(0)
        , period(period)
        , periodFace(200 MILLIS) {
        drawLogo();
        drawFace();
        displayManager->hud->setRenderedBuffer(HUD_BUFFER_SUB);
        //displayManager->hud->drawCenteredText(0, 48, 26, LINK_1);
        //displayManager->hud->drawCenteredText(0, 56, 26, LINK_2);
        //displayManager->hud->drawFrame(0, 58, 128, 6);
    }

    void update() override {
        //displayManager->hud->drawBox(43, 26, map(time, 0, period, 0, 85), 1);
        u8 prevDrawColor = displayManager->hud->getDrawColor();
        displayManager->hud->setDrawColor(2);
        if(time != 0) drawSpinner();
        
        if ((time += deltaTime) >= period) {
            time = 0;
            displayManager->hud->setDrawColor(prevDrawColor);
            displayManager->hud->setRenderedBuffer(HUD_BUFFER_MAIN);
            finishFunction();
            return;
        }
        
        drawSpinner();
        displayManager->hud->setDrawColor(prevDrawColor);

        if ((timeFace += deltaTime) >= periodFace) {
            timeFace = 0;
            drawFace();
        }
    }

    void handleInput(u8 x, u8 y) override {
        time = 0;
        displayManager->hud->setRenderedBuffer(HUD_BUFFER_MAIN);
        finishFunction();
    }
};