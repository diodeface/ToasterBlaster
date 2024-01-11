#pragma once
#include <algorithm>
#include <vector>

#include "Animation/Overlay/Overlay.h"
#include "Animation/Sequence/Keyframe.h"
#include "Arduino.h"
#include "Components/ToasterComponent.h"
#include "Display.h"
#include "EEPROM.h"
#include "Peripherals/HUD/HeadsUpDisplay.h"
#include "Peripherals/Matrix/Matrix.h"
#include "Utilities/Typedefs.h"
#include "Utilities/Util.h"

/**
 * Display Manager
 *
 * Handles all Display objects, drawing to them and displaying the result on a LED matrix.
 */
class DisplayManager : public ToasterComponent {
  private:
    bool redrawRequested;
    Matrix* matrix;
    Keyframe* keyframe;  // current keyframe
    std::vector<Display*> displays;         // stores all registered displays
    std::vector<Display*> displaysToApply;  // filled with displays on which the current effect is being applied
    std::vector<Effect*> effectBuffer;
    std::vector<Overlay> overlays;
    std::vector<Effect*> globalEffects;  // global effects that are never cleared

  public:
    HeadsUpDisplay* hud;

    /**
     * Construct a DisplayManager object.
     * @param matrix Object derived from Matrix class.
     * @param hud HeadsUpDisplay object.
     */
    DisplayManager(Matrix* matrix, HeadsUpDisplay* hud, bool enabled = true);

    /**
     * Registers a display if the given position and bitmask are free.
     * @param display Display to add.
     * @return Pointer to display object if bitmask and position are valid, nullptr otherwise.
     */
    Display* addDisplay(Display* display);

    /**
     * Registers all valid displays from a list.
     * @param displays list of Display pointers
     */
    void addDisplays(std::initializer_list<Display*> displays);

    /**
     * Fills and returns displaysToApply vector with displays that correspond to a bitmask.
     * @param displayBitmask Display Bitmask.
     * @return Pointer to vector of applicable displays.
     */
    std::vector<Display*>* getDisplays(DisplayBitmask displayBitmask);

    /**
     * Returns first display object with given bitmask.
     * @param displayBitmask Display bitmask to check.
     * @return Pointer to first display object with corresponding bitmask, nullptr otherwise.
     */
    Display* getDisplayByBitmask(DisplayBitmask displayBitmask);

    /**
     * Returns first display object with given position.
     * @param position Display position to check.
     * @return Pointer to first display object with corresponding position, nullptr otherwise.
     */
    Display* getDisplayByPosition(u8 position);

    /**
     * Add a global effect. These are applied at the very end of each draw loop.
     * This is the recommended way to do display rotation and mirroring.
     * @param effect Effect object.
     */
    void addGlobalEffect(Effect* effect);

    /**
     * Add an overlay. These are applied over any currently displayed keyframe.
     * @param effect Effect object.
     * @param id Unique id used when deleting multiple effects from the same source.
     */
    void addOverlay(Effect* effect, u8 id);

    /**
     * Remove all overlays with given id.
     * @param id Id of overlays to remove.
     */
    void removeOverlay(u8 id);

    /**
     * Remove all overlays with applyOnRedraw set to false.
     */
    void clearOverlays();

    /**
     * Remove all overlays.
     */
    void clearAllOverlays();

    /**
     * Clear particles on all displays.
     */
    void clearParticles();

    /**
     * Set a keyframe to be displayed on next redraw.
     * @param k Keyframe object.
     */
    void setKeyframe(Keyframe* k);

    /**
     * Redraw all display buffers from a keyframe.
     * @param keyframe Keyframe object.
     */
    void redraw(Keyframe* keyframe);

    /**
     * Redraw all display buffers from what's currently stored and apply new effects.
     * @param effects Vector of Effect objects.
     */
    void redraw(std::vector<Effect*>& effects);

    /**
     * Redraw all display buffers and effects from what's currently stored.
     */
    void redraw();

    /**
     * Enable redraw flag on all buffers, doesn't push to display.
     */
    void requestRedraw();

    /**
     * Show resulting image on a display, draw preview to HUD.
     * @param display Display object to be refreshed.
     */
    void finalize(Display* display);

    /**
     * Show resulting image on all displays, draw preview to HUD.
     */
    void finalize();

    /**
     * Show resulting image on a display, draw preview to HUD.
     * This forces all columns to be displayed no matter if the contents are updated.
     * @param display Display object to be refreshed.
     */
    void forceFinalize(Display* display);
    /**
     * Show resulting image on all displays, draw preview to HUD.
     * This forces all columns to be displayed no matter if the contents are updated.
     */
    void forceFinalize();

    /**
     * @return Matrix brightness
     */
    u8 getMatrixBrightness();

    /**
     * Set matrix brightness to a value between 0 and 15.
     * @param value New brightness value.
     */
    void setMatrixBrightness(u8 value);

    /**
     * Add to current brightness value.
     * @param value Value to add.
     */
    void addMatrixBrightness(i8 value);

    // todo: documentation
    void start() override;

    void stop() override;

    void pause() override;

    void reset() override;

    /**
     * Main update loop.
     * Checks if either a new keyframe has been set, or a redraw has been requested,
     * if either is true the display buffers are redrawn and shown.
     */
    void update();
};