# Gamepad button mapping
- Button mapping can be changed in the `createButtonMapping()` function in [main.cpp](../src/main.cpp).
- The program maps gamepad inputs onto [virtual buttons](../src/System/Peripherals/Gamepad/Button.h) according to these tables:

#### Xbox Wireless Gamepad

| Button | Mapping |
| - | - |
| Left joystick | Joystick 0 |
| Right joystick | Joystick 1 |
| Directional pad | BTN_UP, BTN_UP_RIGHT, BTN_RIGHT, BTN_DOWN_RIGHT, BTN_DOWN, BTN_DOWN_LEFT, BTN_LEFT, BTN_UP_LEFT |
| A, B, X, Y | BTN_A, BTN_B, BTN_X, BTN_Y |
| LB, RB | BTN_L1, BTN_R1 |
| LT, RT | BTN_L2, BTN_R2 |
| LS, RS | BTN_L3, BTN_R3 |
| View, Menu, Share, Xbox button | BTN_SELECT, BTN_START, BTN_SHARE, BTN_HOME |

#### MOCUTE-052f
This gamepad has two modes of operation: GAME and KEY. Change between GAME and KEY mode with the switch on the left side of the gamepad. In KEY mode, the joystick can be toggled between directional pad and analog joystick with the Menu button. I recommend only using GAME mode to avoid confusion.

| Button | Mapping - GAME mode | Mapping - KEY mode | 
| - | - | - |
| Joystick | Joystick 0 | Joystick 1 (joystick) or BTN_UP, BTN_RIGHT, BTN_DOWN, BTN_LEFT (dpad) |
| Menu | BTN_START | Toggle between to joystick and dpad mode | 
| Back/Power | BTN_SELECT | BTN_L3 |
| A | BTN_A | BTN_L2 |
| B | BTN_B | BTN_SHARE |
| X | BTN_X | BTN_HOME |
| Y | BTN_Y | BTN_R2 |
| OK (Trigger) | BTN_R1 | BTN_L1 |
