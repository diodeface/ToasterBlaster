#pragma once

#include "Typedefs.h"
#include "Controller.h"
#include "System/Animation/Sequence/Sequence.h"
#include "System/Animation/Sequence/SequencePlayer.h"

using namespace Config::Displays;

/**
 * Face switcher
 *
 * Maps 4 face sequences to 4 joystick buttons.
 */
class Controller_FaceSwitcher : public Controller {
  private:
    Sequence* sequences[8];
    DisplayManager* displayManager;
    SequencePlayer* sequencePlayer;
    const u8 lineXCenter = 64, lineYCenter = 40;
    u8 prevLineX, prevLineY, prevOctant, prevSequence;
    bool joystickCentered;

    const HUDElement::Element hudPreviewPositions[8] = {
        {0, 32},
        {0, 16},
        {43, 16},
        {86, 16},
        {86, 32},
        {86, 48},
        {43, 48},
        {0, 48},
    };

    void setSequence(u8 sequenceNumber) {
        prevLineX = lineXCenter;
        prevLineY = lineYCenter; 
        if(sequenceNumber == prevSequence) return;
        prevSequence = sequenceNumber;
        sequencePlayer->transitionSequence(sequences[sequenceNumber]);
    }

    void initHUD() {
        displayManager->hud->clearBottom();
        for(u8 i = 0; i < (sizeof(sequences) / sizeof(Sequence*)); i++) {
            // draw icons
            if(Bitmap* bitmap = sequences[i]->keyframes[0].getBitmap(EYE_L)) {
                displayManager->hud->drawBitmap(bitmap, 16, hudPreviewPositions[i].x + 7, hudPreviewPositions[i].y);
            }
            if(Bitmap* bitmap = sequences[i]->keyframes[0].getBitmap(NOSE_L)) {
                displayManager->hud->drawBitmap(bitmap, 8, hudPreviewPositions[i].x + 29, hudPreviewPositions[i].y);
            }
            if(Bitmap* bitmap = sequences[i]->keyframes[0].getBitmap(MOUTH_L)) {
                displayManager->hud->drawBitmap(bitmap, 32, hudPreviewPositions[i].x + 5, hudPreviewPositions[i].y + 8);
            }
            if(sequences[i] == sequencePlayer->getSequence()) {
                u8 prevDrawColor = displayManager->hud->getDrawColor();
                displayManager->hud->setDrawColor(2);
                displayManager->hud->drawBox(hudPreviewPositions[i].x, hudPreviewPositions[i].y, 42, 16);
                displayManager->hud->setDrawColor(prevDrawColor);
            }
        }
    }

  public:
    Controller_FaceSwitcher(
        DisplayManager* displayManager, SequencePlayer* sequencePlayer, Sequence* f0, Sequence* f1, Sequence* f2,
        Sequence* f3, Sequence* f4, Sequence* f5, Sequence* f6, Sequence* f7
    )
        : Controller(CONTROLLER_FACE_SWITCHER, SP_NONE)
        , displayManager(displayManager)
        , sequencePlayer(sequencePlayer)
        , prevLineX(lineXCenter)
        , prevLineY(lineYCenter)
        , prevOctant(255)
        , prevSequence(255)
        , joystickCentered(true)
        , sequences{f0, f1, f2, f3, f4, f5, f6, f7} {
        initHUD();
        TOASTER_LOG("Controller_FaceSwitcher initialized with faces: %s, %s, %s, %s, %s, %s, %s, %s\n", f0->name, f1->name, f2->name, f3->name, f4->name, f5->name, f6->name, f7->name);
    }

    void handleRawInput(u8 x, u8 y) override {
        u8 lineX = map(x, 0, 255, 0, 128);
        u8 lineY = map(y, 0, 255, 16, 64);

        int16_t dx = 128 - (int16_t)x;
        int16_t dy = 128 - (int16_t)y;
        
        if(abs(dx) < 64 && abs(dy) < 64) {
            setSequence(prevOctant);
            if (!joystickCentered) initHUD();   // redraw everything on joystick center
            joystickCentered = true;
            return;
        }

        joystickCentered = false;

        float angleRad = atan2(dy, dx);
        float angleDeg = angleRad * (180.0 / M_PI);

        if (angleDeg < 0) angleDeg += 360.0;

        uint8_t octant = (uint8_t)((angleDeg + 22.5) / 45.0) % 8;

        u8 prevDrawColor = displayManager->hud->getDrawColor();
        displayManager->hud->setDrawColor(2);
        displayManager->hud->drawLine(64, 40, prevLineX, prevLineY);
        displayManager->hud->drawLine(64, 40, lineX, lineY);
        if(prevOctant != octant) {
            displayManager->hud->setDrawColor(0);
            for(u8 i = 0; i < (sizeof(sequences) / sizeof(Sequence*)); i++)displayManager->hud->drawCross(hudPreviewPositions[i].x, hudPreviewPositions[i].y, 42, 16, 3);
            displayManager->hud->setDrawColor(1);
            displayManager->hud->drawCross(hudPreviewPositions[octant].x, hudPreviewPositions[octant].y, 42, 16, 3);
            prevOctant = octant;
            displayManager->hud->drawStatusText(sequences[octant]->name);
        }
        displayManager->hud->setDrawColor(prevDrawColor);
        
        prevLineX = lineX;
        prevLineY = lineY;
    }
};