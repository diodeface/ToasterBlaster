#include "InputHandler.h"

InputHandler::InputHandler() {
    TOASTER_LOG("InputHandler initialized.");
    queuedInputs.reserve(32);
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

void InputHandler::mapButtonsToJoystick(Button up, Button down, Button left, Button right, std::function<void(u8, u8)> callback) {
    mapButton({up, BTN_STATE_PRESSED, [callback]() {callback(0x80, 0x00);}});
    mapButton({down, BTN_STATE_PRESSED, [callback]() {callback(0x80, 0xff);}});
    mapButton({left, BTN_STATE_PRESSED, [callback]() {callback(0xff, 0x80);}});
    mapButton({right, BTN_STATE_PRESSED, [callback]() {callback(0x00, 0x80);}});

    mapButton({up, BTN_STATE_RELEASED, [callback]() {callback(0x80, 0x80);}});
    mapButton({down, BTN_STATE_RELEASED, [callback]() {callback(0x80, 0x80);}});
    mapButton({left, BTN_STATE_RELEASED, [callback]() {callback(0x80, 0x80);}});
    mapButton({right, BTN_STATE_RELEASED, [callback]() {callback(0x80, 0x80);}});
}
    
void InputHandler::update() {
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