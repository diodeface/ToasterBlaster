#pragma once

// todo: documentation

#include <vector>

#include "Utilities/Typedefs.h"
#include "Components/ToasterComponent.h"
#include "Tweenable.h"
#include "TweenFunction.h"

class DisplayManager;

/**
 * Tween manager
 * Manages Tweenable objects (interpolation)
 */
class TweenManager : public ToasterComponent {
  private:
    DisplayManager* displayManager;
    std::vector<ITweenable*> tweens;

  public:
    TweenManager(DisplayManager* displayManager, u8 initialLimit = 24);

    void add(ITweenable* t);

    void clearAll();

    void clearInterruptable();

    void update() override;
};