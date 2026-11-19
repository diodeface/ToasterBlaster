/**
 * Adeon's Toaster Blaster
 * ---------------------------------------------------------------------------
 * This is an LED control program for protogen helmets to be used with MAX7219
 * LED matrixes and ESP32 dev boards. Keyframe animation system allows you
 * to create animations with ease. Facial expressions are controlled with an
 * inexpensive bluetooth controller (MOCUTE-052F), with more gamepads to be
 * supported in the future.
 * 
 * This is a pre-release with some bugs, unfinished code and 
 * lacking documentation. Use at your own risk.
 */

#include <Arduino.h>
#include "main.h"

Matrix* matrix;
DisplayManager* displayManager;
SequencePlayer* sequencePlayer;
Controller* controller;
TaskHandle_t asyncLoopTask;
Timestamp lastTime;

SettingsManager* settingsManager;
HeadsUpDisplay* hud;
TweenManager* displayTweenManager;
EyeBlink* eyeBlink;

VacuumCleanerButton* vacuumCleanerButton;

void setup() { 
    #ifdef DEBUG
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    #endif
    TOASTER_LOG("\n\n\n\n\n\n%s %s\n", PROJECT_NAME, VERSION_NUMBER);
    TOASTER_LOG("Compiled %s\n", COMPILE_TIMESTAMP);
    TOASTER_LOG("Bitmaps compiled %s\n", Bitmaps::BITMAP_GENERATION_TIMESTAMP);
    TOASTER_LOG("%s%s\n\n", LINK_1, LINK_2);

    EEPROM.begin(SETTING_COUNT + 4);    // Settings checksum is stored at address `SETTING_COUNT`

    hud = new DummyHUD();
    //matrix = new Matrix_Max7219(Config::Pins::MATRIX_DIN, Config::Pins::MATRIX_CS, Config::Pins::MATRIX_CLK, SettingsManager::getSavedValue(SETTING_MATRIX_BRIGHTNESS));
    matrix = new Matrix_Max7219(Config::Pins::MATRIX_DIN, Config::Pins::MATRIX_CS, Config::Pins::MATRIX_CLK, 0);
    matrix->clear();
    displayManager = new DisplayManager(matrix, hud, SettingsManager::getSavedValue(SETTING_ENABLE_MATRIX));
    //displayManager = new DisplayManager(matrix, hud, true);
    displayTweenManager = new TweenManager([](){displayManager->requestRedraw();});

    // Register displays (matrix position, number of rows (size), bitmask, hud preview position)
    // Simply comment out any displays that are not in use.
    using namespace Config::Displays;
    displayManager->addDisplays({
        new Display(POS_EYE_L,    16,   EYE_L,    {0,  0},     displayTweenManager),
        new Display(POS_EYE_R,    16,   EYE_R,    {16, 0},     displayTweenManager)
    });

    // To add persistent rotation or other effects to your displays, add a global effect:
    //displayManager->addGlobalEffect(new Rotate180(ALL));

    // initialize eye blink animation
    eyeBlink = new EyeBlink(
        displayManager,                     // displayManager instance
        &Overlays::EyeBlink::closing,       // closing overlay
        &Overlays::EyeBlink::closed,        // closed overlay
        &Overlays::EyeBlink::opening,       // opening overlay
        5 SECONDS,                          // minimum delay between auto blinks
        20 SECONDS,                         // maximum delay between auto blinks
        40 MILLIS,                          // minimum closed eyes duration
        200 MILLIS,                         // maximum closed eyes duration
        SettingsManager::getSavedValue(SETTING_ENABLE_AUTO_BLINK)       // enabled
    );

    // initialize sequencePlayer with startup sequence
    sequencePlayer = new SequencePlayer(displayManager, displayTweenManager, eyeBlink, &Sequences::powerstickDefault, 0, true);

    vacuumCleanerButton = new VacuumCleanerButton(6, sequencePlayer);

    // initialize default controller
    controller = new Controller_Startup(displayManager, 3 SECONDS, [](){changeController(new Controller_Settings(displayManager, settingsManager));});
}

// Main program loop
void loop(){
    // calculate delta time
    deltaTime = (Timestamp)(micros() - lastTime);
    if(deltaTime < Config::FRAMETIME) return;       // intentionally limiting updates per second to avoid fixed point imprecision issues
    lastTime = micros();
    sequencePlayer->update();
    controller->update();
    displayTweenManager->update();
    eyeBlink->update();
    displayManager->update();
    vacuumCleanerButton->update();
}

// Controller related functions
void changeController(Controller* newController) {
    delete controller;
    switch(newController->getSequencePlayerState()){
        case SP_PLAY: sequencePlayer->start(); break;
        case SP_STOP: sequencePlayer->stop(); break;
    }
    controller = newController;
}

void createSettingsMenu() {
    settingsManager->addSettings({
        new Setting_Lambda(
            "Matrix",       // category
            "Brightness",   // name
            SETTING_MATRIX_BRIGHTNESS,  // unique key defined in Settings/SettingKeys.h enum SettingKey
            SETTING_TYPE_PERCENTAGE,    // setting type, as defined in Settings/Setting.h enum SettingType
            0, 0, 15, 1,        // default, minimum, maximum, increment values
            []() -> u8 { return displayManager->getMatrixBrightness(); },       // getter returning u8
            [](u8 value) { displayManager->setMatrixBrightness(value); }        // setter using u8
        ),

        new Setting_ComponentToggle("Matrix", "Enable", SETTING_ENABLE_MATRIX, true, displayManager),        // category, name, unique key, default value, child of ToasterComponent

        new Setting_ComponentToggle("Auto blink", "Enable", SETTING_ENABLE_AUTO_BLINK, true, eyeBlink),

        new Setting_Action("System", "Restart", [](u8 value) { ESP.restart(); }),

        new Setting_Action("System", "Save settings", [](u8 value) { settingsManager->saveSettings(); }),
    });
}

void safeMode() {

}