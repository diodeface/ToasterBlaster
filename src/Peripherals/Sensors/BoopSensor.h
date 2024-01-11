/**
 * Boop sensor
 * Overlays a face upon sensor activation
 */

#include "Utilities/Typedefs.h"
#include "Utilities/Globals.h"
#include "Sensor.h"
#include "Drawing/DisplayManager.h"
#include "Animation/Overlay/OverlayPlayer.h"

class BoopSensor {
    bool boop;
    OverlayPlayer* overlay;
    OverlaySequence *enableOverlay, *disableOverlay;

    public:
    DisplayManager* displayManager;

    void onEnable() {
        if(boop) return;
        boop = true;
        overlay->setSequence(enableOverlay);
        overlay->forceStart();
        boopCount++;
        TOASTER_LOG("Boop! (%d)\n", boopCount);
    }

    void onDisable() {
        if(!boop) return;
        boop = false;
        overlay->setSequence(disableOverlay);
        overlay->forceStart();
        TOASTER_LOG("Boop off.\n", boopCount);
    }

    void onUpdate() {
        overlay->update();
    }

    BoopSensor(
        DisplayManager* displayManager, 
        OverlayPlayer* overlay, 
        OverlaySequence* enableOverlay,
        OverlaySequence* disableOverlay
    )
        : displayManager(displayManager)
        , overlay(overlay)
        , enableOverlay(enableOverlay)
        , disableOverlay(disableOverlay) {
        boop = false;
    }
};