#pragma once

#include "Arduino.h"
#include "System/Components/ToasterComponent.h"
#include "Typedefs.h"
#include "System/Utilities/Globals.h"

class UptimeCounter : public ToasterComponent {
  private:
    Timestamp lastTime;

  public:
    UptimeCounter(bool enabled = true)
        : lastTime(0) 
    {
        this->enabled = enabled;
    }

    void update() override {
        if(millis() - lastTime < 1000) return;
        lastTime = millis();

        Timestamp seconds = millis() / 1000;
        Timestamp minutes = seconds / 60;
        // Timestamp hours = minutes / 60;
        minutes %= 60;
        seconds %= 60;

        // if(seconds < 3600) {
            sprintf(uptime, "%2d%s%02d", minutes, seconds % 2 ? " " : ":", seconds);
        // } else {
        //     sprintf(uptime, "%2d%s%02d", hours, seconds % 2 ? " " : ":", minutes);
        // }
    }
};