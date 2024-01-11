#pragma once

#include "Utilities/Typedefs.h"
#include "Controller.h"
#include "Animation/Sequence/Sequence.h"
#include "Animation/Sequence/SequencePlayer.h"

using namespace Config::Displays;

/**
 * Face switcher
 *
 * Maps 4 face sequences to 4 joystick buttons.
 */
class FaceSwitcherController : public Controller {
  private:
    Sequence* sequences[4];
    DisplayManager* displayManager;
    SequencePlayer* sequencePlayer;

    const HUDElement::Element hudPreviewPositions[4] = {{48, 16}, {16, 32}, {48, 48}, {80, 32}};

    void setSequence(u8 sequenceNumber) {
        if(sequencePlayer->transitionSequence(sequences[sequenceNumber])) {
            initHUD();
        }
    }

    void initHUD() {
        displayManager->hud->clearBottom();
        for(u8 i = 0; i < 4; i++) {
            // draw icons
            if(Bitmap* bitmap = sequences[i]->keyframes[0].getBitmap(EYE_L)) {
                displayManager->hud->drawBitmap(bitmap, 16, hudPreviewPositions[i].x, hudPreviewPositions[i].y);
            }
            if(Bitmap* bitmap = sequences[i]->keyframes[0].getBitmap(EYE_R)) {
                displayManager->hud->drawBitmap(bitmap, 16, hudPreviewPositions[i].x + 19, hudPreviewPositions[i].y);
            }
            // draw names
            displayManager->hud->drawCenteredText(
                hudPreviewPositions[i].x - 16, hudPreviewPositions[i].y + 15, 13, sequences[i]->name
            );
            // highlight if current
            if(sequences[i] == sequencePlayer->getSequence()) {
                u8 prevDrawColor = displayManager->hud->getDrawColor();
                displayManager->hud->setDrawColor(2);
                displayManager->hud->drawBox(hudPreviewPositions[i].x - 16, hudPreviewPositions[i].y, 64, 16);
                displayManager->hud->setDrawColor(prevDrawColor);
            }
        }
    }

  public:
    FaceSwitcherController(
        DisplayManager* displayManager, SequencePlayer* sequencePlayer, Sequence* f0, Sequence* f1, Sequence* f2,
        Sequence* f3
    )
        : Controller(CONTROLLER_FACE_SWITCHER, SP_NONE)
        , displayManager(displayManager)
        , sequencePlayer(sequencePlayer)
        , sequences{f0, f1, f2, f3} {
        initHUD();
        TOASTER_LOG("FaceSwitcherController initialized with faces: %s, %s, %s, %s\n", f0->name, f1->name, f2->name, f3->name);
    }

    void handleInput(u8 x, u8 y) override {
        if(y == 0x00) setSequence(0);
        else if(x == 0xFF) setSequence(1);
        else if(y == 0xFF) setSequence(2);
        else if(x == 0x00) setSequence(3);
    }
};