#pragma once

// todo: documentation

#include <vector>
#include <functional>

#include "Typedefs.h"
#include "System/Components/ToasterComponent.h"
#include "Tweenable.h"
#include "TweenFunction.h"

/**
 * Tween manager
 * Manages Tweenable objects (interpolation)
 */
class TweenManager : public ToasterComponent {
  private:
    std::function<void()> postUpdateFunction;
    std::vector<ITweenable*> tweens;

  public:
    TweenManager(std::function<void()> postUpdateFunction, u8 initialLimit = 24);

    void add(ITweenable* t);

    void clearAll();

    void clearInterruptable();

    void update() override;
};