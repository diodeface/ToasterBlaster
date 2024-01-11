#pragma once

#include "Config.h"
#include "Utilities/Globals.h"
#include "EEPROM.h"

#include "Peripherals/Matrix/Max7219.h"
#include "Animation/Interpolation/TweenManager.h"
#include "Drawing/DisplayManager.h"
#include "Peripherals/HUD/U8G2HUDWrapper.h"

#include "Animation/Sequence/SequencePlayer.h"

#include "Controllers/Controller.h"
#include "Controllers/StartupController.h"
#include "Controllers/SettingsController.h"
#include "Controllers/FaceSwitcherController.h"
#include "Controllers/SnakeGameController.h"
#include "Controllers/AutomaticFaceSwitcherController.h"
#include "Controllers/SpectrumAnalyzerController.h"

#include "Animation/EyeBlink.h"

#include "Peripherals/Microphone/AnimatedMouth.h"
#include "Animation/Overlay/OverlayPlayer.h"
#include "Peripherals/Sensors/AnalogBoopSensor.h"

#include "Peripherals/LEDStrip.h"

#include "Components/UptimeCounter.h"

extern SettingsManager* settingsManager;
extern SequencePlayer* sequencePlayer;
extern DisplayManager* displayManager;
extern EyeBlink* eyeBlink;

#ifdef BT_CONTROLLER
#include "Peripherals/BLEGamepad/BLEGamepad.h"
#endif

#include "Peripherals/PWMFan.h"

#include "Settings/SettingsManager.h"

void changeController(Controller* newController);
void loopAsync(void* pvParameters);
void handleControllerInput(u8 x, u8 y);
void createSettingsMenu();
void createButtonMapping();