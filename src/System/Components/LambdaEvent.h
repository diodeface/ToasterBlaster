#pragma once

#include "Arduino.h"
#include "System/Components/ToasterComponent.h"
#include "Typedefs.h"

// todo: start, stop, pause, reset

class LambdaEvent : public ToasterComponent {
  private:
    Timestamp lastTime;
    Timestamp period;
    std::function<void()> function;

  public:
    LambdaEvent(
        Timestamp period,
        std::function<void()> function
    )
        : function(function) 
    {

    }

    void update() override {
        if(millis() - lastTime < period) return;
        function();
    }
};