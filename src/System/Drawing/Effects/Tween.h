#pragma once

#include "Typedefs.h"
#include "Effect.h"
#include "System/Animation/Interpolation/TweenManager.h"

template <typename T>
class Tween : public Effect {
  private:
    T* pointer;
    f32 start, end, duration;
    bool rounding, interruptable;
    TweenFunction tweenFunction;

  public:
    /**
     * Initiates a tween.
     * @param pointer Pointer to interpolated variable
     * @param start Start value
     * @param end End value
     * @param duration Tween duration
     * @param tweenFunction Tween function
     * @param interruptable True if the tween should be finished on sequence/controller change, false otherwise
     * @param rounding True if result should be rounded
     */
    Tween(
        T* pointer, f32 start, f32 end, f32 duration, TweenFunction tweenFunction, bool interruptable = true,
        bool rounding = true
    )
        : Effect(Config::Displays::DEFAULT_DISPLAY, false)
        , pointer(pointer)
        , start(start)
        , end(end)
        , duration(duration)
        , tweenFunction(tweenFunction)
        , rounding(rounding)
        , interruptable(interruptable) {
    }

    void apply(u8* buffer, Display* display) override {
        Effect::apply(buffer, display);
        *pointer = start;
        display->tweenManager->add(
            new Tweenable<T>(pointer, start, end, duration, tweenFunction, interruptable, rounding)
        );
    }
};