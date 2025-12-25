#include "System/Utilities/Util.h"
//#include "System/Controllers/Controller.h"
#include "InputHandler.h"

InputHandler::InputHandler() {
    TOASTER_LOG("InputHandler initialized.");
    queuedInputs.reserve(32);
    mappedButtons.reserve(128);
}

void InputHandler::queueInput(ButtonInput input) {
    // genuinely flabbergasted that this works without a mutex
    if(input.button == BTN_NONE) return;  // ignore unknown inputs
    queuedInputs.push_back(input);
}

void InputHandler::mapButton(ButtonMapping buttonMapping) {
    mappedButtons.push_back(buttonMapping);
}

void InputHandler::mapButtons(std::initializer_list<ButtonMapping> buttonMappings) {
    mappedButtons.insert(mappedButtons.end(), buttonMappings);
}

void InputHandler::mapJoystick(u8 joystickIndex, std::function<void(u8, u8)> filteredCallback, std::function<void(u8, u8)> rawCallback) {
    if (joystickIndex >= JOYSTICK_COUNT) {
        TOASTER_LOG("ERROR: Cannot map joystick %d which is out of range, try changing JOYSTICK_COUNT in InputHandler.h.\n", joystickIndex);
        return;
    }

    joysticks[joystickIndex].filteredCallback = filteredCallback;
    joysticks[joystickIndex].rawCallback = rawCallback;
}

// called when joystick position changes
void InputHandler::handleJoystick(u8 joystickIndex, u8 x, u8 y) {
    if (joystickIndex >= JOYSTICK_COUNT) {
        TOASTER_LOG("ERROR: Joystick %d out of range.\n", joystickIndex);
        return;
    }

    joysticks[joystickIndex].x = x;
    joysticks[joystickIndex].y = y;
}

void InputHandler::mapButtonsToJoystick(u8 joystickIndex, Button up, Button right, Button down, Button left) {
    if (joystickIndex >= JOYSTICK_COUNT) {
        TOASTER_LOG("ERROR: Joystick %d out of range.\n", joystickIndex);
        return;
    }
    mapButton({up, BTN_STATE_PRESSED,       [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0x00);}});
    mapButton({right, BTN_STATE_PRESSED,    [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0xff, 0x80);}});
    mapButton({down, BTN_STATE_PRESSED,     [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0xff);}});
    mapButton({left, BTN_STATE_PRESSED,     [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x00, 0x80);}});

    mapButton({up, BTN_STATE_RELEASED,      [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0x80);}});
    mapButton({right, BTN_STATE_RELEASED,   [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0x80);}});
    mapButton({down, BTN_STATE_RELEASED,    [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0x80);}});
    mapButton({left, BTN_STATE_RELEASED,    [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0x80);}});
}

void InputHandler::mapButtonsToJoystick(u8 joystickIndex, Button up, Button upRight, Button right, Button downRight, Button down, Button downLeft, Button left, Button upLeft) {
    if (joystickIndex >= JOYSTICK_COUNT) {
        TOASTER_LOG("ERROR: Joystick %d out of range.\n", joystickIndex);
        return;
    }
    mapButtonsToJoystick(joystickIndex, up, right, down, left);

    mapButton({upRight, BTN_STATE_PRESSED,          [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0xff, 0x00);}});
    mapButton({downRight, BTN_STATE_PRESSED,        [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0xff, 0xff);}});
    mapButton({downLeft, BTN_STATE_PRESSED,         [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x00, 0xff);}});
    mapButton({upLeft, BTN_STATE_PRESSED,           [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x00, 0x00);}});

    mapButton({upRight, BTN_STATE_RELEASED,         [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0x80);}});
    mapButton({downRight, BTN_STATE_RELEASED,       [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0x80);}});
    mapButton({downLeft, BTN_STATE_RELEASED,        [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0x80);}});
    mapButton({upLeft, BTN_STATE_RELEASED,          [joystickIndex, this]() {this->handleJoystick(joystickIndex, 0x80, 0x80);}});
}
    
void InputHandler::update() {
    // handle joysticks
    for(Joystick& j : joysticks) {
        if (j.filteredCallback) {
            u8 rX = roundAnalogValue(j.x);
            u8 rY = roundAnalogValue(j.y);

            if (rX == 0x80) j.debounceX.reset();
            else if (j.debounceX.shouldActivate()) j.filteredCallback(rX, 0x80);

            if (rY == 0x80) j.debounceY.reset();
            else if (j.debounceY.shouldActivate()) j.filteredCallback(0x80, rY);
        }

        if (j.rawCallback) {
            if (j.prevX != j.x || j.prevY != j.y) j.rawCallback(j.x, j.y);
            j.prevX = j.x;
            j.prevY = j.y;
        }
    }

    // handle buttons
    for(ButtonInput& input : queuedInputs) {
        BT_LOG("Button: %d, State: %d\n", input.button, input.state);
        for(ButtonMapping& mapping : mappedButtons) {
            if (input.button == mapping.buttonState.button && input.state == mapping.buttonState.state) {
                mapping.function();
                break;
            }
        }
    }
    queuedInputs.clear();
}