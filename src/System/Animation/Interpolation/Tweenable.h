#pragma once

/**
 * Tweenables allow interpolation of a variable pointed to.
 * Note: Ongoing tweens pointing the same variable will overlap each other
 * and cause weird looking animations. Tweenables therefore have a "interruptable"
 * flag that determines whether or not to clear them on sequence/controller change.
 */

#include "ITweenable.h"

template <typename T>
class Tweenable : public ITweenable {
  private:
    T* pointer;

  public:
    Tweenable(
        T* pointer, f32 start, f32 end, 
        Timestamp duration, 
        TweenFunction tweenFunction, 
        bool interruptable = true,
        bool rounding = true
    );

    bool apply() override;

    void finish() override;
};

#include "Tweenable.tpp"