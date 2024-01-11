#include <vector>
#include "Utilities/Typedefs.h"
#include "Controller.h"
#include "Animation/Sequence/Sequence.h"
#include "Animation/Sequence/SequencePlayer.h"

using namespace Config::Displays;

/**
 * Automatic face switcher
 * 
 * Switches randomly from a set of faces with a time interval
 */

class AutomaticFaceSwitcherController : public Controller {
  private:
    DisplayManager* displayManager;
    SequencePlayer* sequencePlayer;

    std::vector<Sequence*> sequences;
    Sequence* nextSequence;

    Timestamp time, period, minWait, maxWait;
    Timestamp displayRefreshTime;

    void drawSmallFacePreview (Sequence* sequence, u8 x, u8 y) {
        if(Bitmap* bitmap = sequence->keyframes[0].getBitmap(EYE_L)) {
            displayManager->hud->drawBitmap(bitmap, 16, x + 1, y);
        }
        if(Bitmap* bitmap = sequence->keyframes[0].getBitmap(NOSE_L)) {
            displayManager->hud->drawBitmap(bitmap, 8, x + 23, y);
        }
        if(Bitmap* bitmap = sequence->keyframes[0].getBitmap(MOUTH_L)) {
            displayManager->hud->drawBitmap(bitmap, 32, x, y + 8);
        }
    }

    void setNextSequence() {
        if (sequences.size() == 0) return;
        period = random(minWait, maxWait);
        time = 0;

        Sequence* currentSequence = nextSequence;
        while (currentSequence == nextSequence) {
            nextSequence = sequences[random(sequences.size()-1)];
            if (sequences.size() == 1) break;
        }
        
        drawSmallFacePreview(nextSequence, 0, 48);
        displayManager->hud->drawStrX2(38, 63, nextSequence->name);
        TOASTER_LOG("Switching to face %s in %dms.\n", nextSequence->name, period / 1000);
    }

    void goToNextSequence() {
        // draw current sequence
        displayManager->hud->clearBottom();
        sequencePlayer->transitionSequence(nextSequence);
        drawSmallFacePreview(nextSequence, 0, 16);
        displayManager->hud->drawStrX2(38, 31, nextSequence->name);
        
        setNextSequence();
    }

  public:
    AutomaticFaceSwitcherController(
        DisplayManager* displayManager, 
        SequencePlayer* sequencePlayer,
        Timestamp minWait,
        Timestamp maxWait,
        const std::vector<Sequence*> &sequences
    ) 
        : Controller(CONTROLLER_AUTOMATIC_FACE_SWITCHER, SP_NONE)
        , displayManager(displayManager)
        , sequencePlayer(sequencePlayer)
        , sequences(sequences)
        , minWait(minWait)
        , maxWait(maxWait)
        , time(0)
        , displayRefreshTime(0){
        displayManager->hud->clearBottom();
        setNextSequence();
        TOASTER_LOG("Automatic face switcher initialized with %d faces.\n", sequences.size());
    }

    void update() override {
        if ((displayRefreshTime += deltaTime) > 100 MILLIS) {
            displayRefreshTime = 0;
            char buf[16];
            sprintf(buf, "%.1f", (period - time) / 1000000.0f);
            displayManager->hud->drawStrX2(0, 47, buf);
        }

        if((time += deltaTime) < period) return;
        goToNextSequence();
    }
    
    void handleInput(u8 x, u8 y) override {
        if(time < 250 MILLIS) return;
        goToNextSequence();
    }
};