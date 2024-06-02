#pragma once

#include "Utilities/Typedefs.h"
#include "Utilities/Util.h"
#include "Components/ToasterComponent.h"
#include "Button.h"
#include <functional>
#include <vector>

struct ButtonInput {
    Button button;
    u8 state;
};

struct ButtonMapping {
    ButtonInput buttonState;
    std::function<void()> function;
};

class InputHandler : public ToasterComponent {
  private:
    std::vector<ButtonMapping> mappedButtons;
    std::vector<ButtonInput> queuedInputs;
  public:
    InputHandler();
    void queueInput(ButtonInput input);
    void mapButton(ButtonMapping buttonMapping);
    void mapButtons(std::initializer_list<ButtonMapping> buttonMappings);
    void mapButtonsToJoystick(Button up, Button down, Button left, Button right, std::function<void(u8, u8)> callback);
    void update() override;
};