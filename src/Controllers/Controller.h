#pragma once

/**
 * Base controller class.
 *
 * The purpose of controllers is reacting to joystick inputs and controlling
 * the various components accordingly.
 */

#include "Utilities/Typedefs.h"
#include "Arduino.h"
#include "Components/ToasterComponent.h"

enum ControllerType {
    CONTROLLER_DEFAULT,
    CONTROLLER_STATUS,
    CONTROLLER_SLIT_EYE,
    CONTROLLER_FACE_SWITCHER,
    CONTROLLER_AUTOMATIC_FACE_SWITCHER,
    CONTROLLER_SPECTRUM_ANALYZER
};

enum SequencePlayerState {
    SP_STOP,    // stop sequence player on controller change
    SP_PLAY,    // play sequence player on controller change
    SP_NONE     // keep sequence player state
};

class Controller : public ToasterComponent {
  protected:
    ControllerType type;
    SequencePlayerState sequencePlayerState;

  public:
    Controller() {
    }

    virtual ~Controller() {
    }

    Controller(ControllerType type = CONTROLLER_DEFAULT, SequencePlayerState sequencePlayerState = SP_NONE)
        : type(type)
        , sequencePlayerState(sequencePlayerState) {
    }

    virtual void update() override {
    }

    virtual void handleInput(u8 x, u8 y) {
    }
    
    ControllerType getType() {
        return type;
    }

    SequencePlayerState getSequencePlayerState() {
        return sequencePlayerState;
    }
};
