#ifdef GAMEPAD_MOCUTE052F
#include "System/Peripherals/Gamepad/InputConfig_Mocute052f.h"
#define BT_GAMEPAD
#endif

#ifdef GAMEPAD_XBOX
#include "System/Peripherals/Gamepad/InputConfig_Xbox.h"
#define BT_GAMEPAD
#endif

#ifdef GAMEPAD_DEBUG
#include "System/Peripherals/Gamepad/InputConfig_Debug.h"
#define BT_GAMEPAD
#endif

#ifdef BT_GAMEPAD
#include "System/Peripherals/Gamepad/Gamepad.h"
#endif