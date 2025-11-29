template <typename T>
Tweenable<T>::Tweenable(
    T* pointer, f32 start, f32 end, 
    Timestamp duration, 
    TweenFunction tweenFunction, 
    bool interruptable,
    bool rounding
)
    : pointer(pointer)
    , ITweenable(start, end, duration, tweenFunction, interruptable, rounding) {
}

template <typename T>
bool Tweenable<T>::apply() {
    timer += (f32)deltaTime / (f32)duration;
 
    if(timer >= 1.0f) {
        *pointer = end;
        return true;
    }

    *pointer = rounding ? nearbyint(lerp(start, end, getTweenFunction(tweenFunction, timer)))
                        : lerp(start, end, getTweenFunction(tweenFunction, timer));

    return false;
}

template <typename T>
void Tweenable<T>::finish() {
    *pointer = end;
    timer = 1.0f;
}