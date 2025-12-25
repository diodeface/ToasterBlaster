#pragma once

#include <vector>
#include <functional>

#include "Typedefs.h"
#include "System/Utilities/Util.h"

#include "Button.h"
#include "ButtonDebounce.h"

static const u8 JOY_CENTER = 0x80;

struct ButtonInput {
    Button button;
    u8 state;
};

struct Joystick {
    u8 x, prevX;
    u8 y, prevY;
    ButtonDebounce debounceX, debounceY;
    std::function<void(u8, u8)> filteredCallback;
    std::function<void(u8, u8)> rawCallback;

    Joystick(
        u8 x = JOY_CENTER,
		u8 prevX = JOY_CENTER,
		u8 y = JOY_CENTER,
		u8 prevY = JOY_CENTER,
		ButtonDebounce debounceX = ButtonDebounce(),
		ButtonDebounce debounceY = ButtonDebounce(),
		std::function<void(u8, u8)> filteredCallback = nullptr,
		std::function<void(u8, u8)> rawCallback = nullptr
    ) 
		: x(x)
		, prevX(prevX)
		, y(y)
		, prevY(prevY)
		, debounceX(debounceX)
		, debounceY(debounceY)
		, filteredCallback(filteredCallback)
		, rawCallback(rawCallback)
	{

	}
};

struct ButtonMapping {
    ButtonInput buttonState;
    std::function<void()> function;
};

class InputHandler {
  private:
    static constexpr u8 JOYSTICK_COUNT = 2;
    std::vector<ButtonMapping> mappedButtons;
    std::vector<ButtonInput> queuedInputs;
    Joystick joysticks[JOYSTICK_COUNT];
  public:
    InputHandler();
    void queueInput(ButtonInput input);
    void mapButton(ButtonMapping buttonMapping);
    void mapButtons(std::initializer_list<ButtonMapping> buttonMappings);
    void mapJoystick(u8 joystickIndex, std::function<void(u8, u8)> filteredCallback, std::function<void(u8, u8)> rawCallback);
    void handleJoystick(u8 joystickIndex, u8 x, u8 y);
    void mapButtonsToJoystick(u8 joystickIndex, Button up, Button right, Button down, Button left);
    void mapButtonsToJoystick(u8 joystickIndex, Button up, Button upRight, Button right, Button downRight, Button down, Button downLeft, Button left, Button upLeft);
    void update();
};