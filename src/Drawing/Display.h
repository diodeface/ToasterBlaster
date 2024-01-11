#pragma once

#include "Utilities/Typedefs.h"
#include <vector>
#include "Arduino.h"
#include "Utilities/Util.h"
#include "Drawing/Particles/ParticleSystem.h"
#include "Animation/Interpolation/TweenManager.h"

class Effect;
class Overlay;

/**
 * Display
 *
 * Holds information about the display and buffers for the currently displayed bitmap.
 */
class Display {
  private:
    DisplayBitmask displayBitmask;  // unique bit for this panel, used as identifier
    u8 position;                    // row where the display starts, also used as unique identifier
    u8 size;                        // amount of 8 pixel rows, used when creating buffers

  public:
    ParticleSystem* particleSystem;
    TweenManager* tweenManager;     // global TweenManager instance, used when applying effects (todo: this is hacky as hell, use singleton pattern or something idk)
    Point hudPreviewPosition;       // x, y offset on the HUD preview

    // buffers
    u8* previousFrame;  // previous rendered frame, to skip drawing unchanged lines to the screen
    u8* bitmapBuffer;   // main bitmap without any effects applied
    u8* hudBuffer;      // after applying effects
    u8* matrixBuffer;   // after applying overlays that are ignored on HUD

    /**
     * Construct a Display object.
     * @param position Starting row.
     * @param size Size of the display (Amount of 8 pixel rows).
     * @param displayBitmask Unique bit in an integer under which this display is identified.
     * @param hudPreviewPosition Offset for the preview on heads up display.
     * @param tweenManager TweenManager instance.
     */
    Display(u8 position, u8 size, DisplayBitmask displayBitmask, Point hudPreviewPosition, TweenManager* tweenManager);

    ~Display();

    /**
     * Render all particles for this display
     */
    void renderParticles();

    /**
     * Copy an array to another array with the same size as the display.
     * @param from Source
     * @param to Target
     */
    void copy(const u8* from, u8* to);

    /**
     * Compare two arrays with the same size as the display.
     * @param buf1 First array
     * @param buf2 Second array
     */
    bool compare(const u8* buf1, const u8* buf2);

    /**
     * Copy a bitmap with the displays size to its bitmap buffer.
     * @param bitmap Bitmap (Array of u8) to copy.
     */
    void setBitmap(const Bitmap* bitmap);

    /**
     * Apply an effect to a buffer
     * @param buffer Buffer on which the effect is applied
     * @param effect Effect object
     */
    void applyEffect(u8* buffer, Effect* effect);

    /**
     * Apply an overlay to a buffer
     * @param buffer Buffer on which the overlay is applied
     * @param effect Overlay object
     */
    void applyOverlay(u8* buffer, Overlay& overlay);

    /**
     * @return Display position
     */
    u8 getPosition();

    /**
     * @return Display size
     */
    u8 getSize();

    /**
     * @return Display bitmask
     */
    DisplayBitmask getBitmask();
};