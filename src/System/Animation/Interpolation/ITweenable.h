#include "System/Utilities/Util.h"
#include "System/Utilities/Globals.h"
#include "TweenFunction.h"

class ITweenable {
  protected:
    f32 start, end;
    Timestamp duration;
    TweenFunction tweenFunction;
    f32 timer;
    bool rounding;

  public:
    bool interruptable;
    ITweenable(
        f32 start, f32 end, 
        Timestamp duration, 
        TweenFunction tweenFunction, 
        bool interruptable = true,
        bool rounding = true
    );

    virtual bool apply();

    virtual void finish();
};