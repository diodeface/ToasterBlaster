#pragma once

#include "EEPROM.h"
#include "config.h"

#include "System/System.h"

#include "Assets/System.h"
#include "Assets/Standard.h"

extern SettingsManager* settingsManager;
extern SequencePlayer* sequencePlayer;
extern DisplayManager* displayManager;
extern EyeBlink* eyeBlink;
extern InputHandler inputHandler;

void changeController(Controller* newController);
void loopAsync(void* pvParameters);
void handleJoystickInput(u8 x, u8 y);
void createSettingsMenu();
void createButtonMapping();
void safeMode();

#include "System/Gamepads.h"