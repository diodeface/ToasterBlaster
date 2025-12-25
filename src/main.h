#pragma once

#include "EEPROM.h"
#include "config.h"

#include "System/System.h"
#include "System/Gamepads.h"

#include "Assets/System.h"
#include "Assets/Standard.h"

extern SettingsManager* settingsManager;
extern SequencePlayer* sequencePlayer;
extern DisplayManager* displayManager;
extern EyeBlink* eyeBlink;
extern InputHandler inputHandler;

void changeController(Controller* newController);
void loopAsync(void* pvParameters);
void createSettingsMenu();
void createButtonMapping();
void safeMode();
