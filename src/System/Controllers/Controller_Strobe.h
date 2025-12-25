#pragma once
#include <vector>
#include "Typedefs.h"
#include "Controller.h"

#include "System/Utilities/Globals.h"
#include "System/Animation/Sequence/Sequence.h"
#include "System/Animation/Sequence/SequencePlayer.h"
#include "Assets/Sequences/Standard.h"

// todo: finish this
// this is more or less a proof of concept
class Controller_Strobe : public Controller {
  private:
    std::vector<Sequence*> selectedSequences;
    DisplayManager* displayManager;
    SequencePlayer* sequencePlayer;
    TweenManager* tweenManager;
    u8 currentSequence;
    Timestamp time, strobeDuration;

    enum Controller_StrobeState {
        STROBE_SELECT,
        STROBE_STOPPED,
        STROBE_SHOWING
    } state;

  public:
    Controller_Strobe(
        DisplayManager* displayManager, 
        SequencePlayer* sequencePlayer,
        TweenManager* tweenManager
    ) 
        : Controller(CONTROLLER_STROBE)
        , displayManager(displayManager)
        , sequencePlayer(sequencePlayer)
        , tweenManager(tweenManager) {
        selectedSequences.push_back(&Sequences::neutral);
        selectedSequences.push_back(&Sequences::joy);
        selectedSequences.push_back(&Sequences::heartEyes);
        selectedSequences.push_back(&Sequences::dizzy);
        state = STROBE_STOPPED;
        time = 0;
        currentSequence = 0;
        strobeDuration = 500 MILLIS;
        sequencePlayer->stop();
        displayManager->clearDisplays();
    }

    void handleInput(u8 x, u8 y) {
        if (state == STROBE_SHOWING) return;

        for(Display* d : *displayManager->getDisplays(Config::Displays::ALL)) {
            displayManager->redraw(&(selectedSequences[currentSequence]->keyframes[0]));
        }
        tweenManager->clearAll();
        displayManager->forceFinalize();
        
        time = 0;
        state = STROBE_SHOWING;
        currentSequence = (currentSequence + 1) % selectedSequences.size();
        return;

        if(y == 0x00) {}
        else if(y == 0xFF) {}
        
        if(x == 0xFF) {}
        else if(x == 0x00) {}
    }

    void update() override {
        if ((time += deltaTime) > strobeDuration && state == STROBE_SHOWING) {
            displayManager->clearDisplays();
            state = STROBE_STOPPED;
        }
    }
};