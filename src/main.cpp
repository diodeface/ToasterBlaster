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
AnimatedMouth* animatedMouth;

SettingsManager* settingsManager;
HeadsUpDisplay* hud;
TweenManager* displayTweenManager;
AnalogBoopSensor* boopSensor;
EyeBlink* eyeBlink;
LEDStrip* ledStrip;
UptimeCounter* uptimeCounter;
PWMFan* fanControl;
InputHandler inputHandler;

void setup() {
    TOASTER_LOG("\n\n\n\n\n\n%s %s\n", PROJECT_NAME, VERSION_NUMBER);
    TOASTER_LOG("Compiled %s\n", COMPILE_TIMESTAMP);
    TOASTER_LOG("%s%s\n\n", LINK_1, LINK_2);

    EEPROM.begin(SETTING_COUNT + 4);    // Settings checksum is stored at address `SETTING_COUNT`

    hud = new U8G2HUDWrapper(Config::Pins::HUD_SCL, Config::Pins::HUD_SDA);
    matrix = new Max7219(Config::Pins::MATRIX_DIN, Config::Pins::MATRIX_CS, Config::Pins::MATRIX_CLK, SettingsManager::getSavedValue(SETTING_MATRIX_BRIGHTNESS));
    matrix->clear();
    displayManager = new DisplayManager(matrix, hud, SettingsManager::getSavedValue(SETTING_ENABLE_MATRIX));
    displayTweenManager = new TweenManager([](){displayManager->requestRedraw();});

    // Register displays (matrix position, number of rows (size), bitmask, hud preview position)
    // Simply comment out any displays that are not in use.
    using namespace Config::Displays;
    displayManager->addDisplays({
        new Display(POS_EYE_L,    16,   EYE_L,    {49, 0},    displayTweenManager),
        new Display(POS_EYE_R,    16,   EYE_R,    {1, 0},     displayTweenManager),
        new Display(POS_MOUTH_L,  32,   MOUTH_L,  {34, 8},    displayTweenManager),
        new Display(POS_MOUTH_R,  32,   MOUTH_R,  {0, 8},     displayTweenManager),
        new Display(POS_NOSE_L,   8,    NOSE_L,   {35, 0},    displayTweenManager),
        new Display(POS_NOSE_R,   8,    NOSE_R,   {23, 0},    displayTweenManager),
    });

    // To add persistent rotation or other effects to your displays, add a global effect:
    //displayManager->addGlobalEffect(new Rotate180(ALL));

    //ledStrip = new LEDStrip(SettingsManager::getSavedValue(SETTING_ENABLE_LEDSTRIP));     // spinning color LED strip animation
    ledStrip = new LEDStripCD(SettingsManager::getSavedValue(SETTING_ENABLE_LEDSTRIP));     // optical disc LED strip animation

    #ifdef BT_GAMEPAD
    Gamepad::init();
    createButtonMapping();
    #endif

    eyeBlink = new EyeBlink(
        displayManager,
        &Overlays::EyeBlink::closing,
        &Overlays::EyeBlink::closed,
        &Overlays::EyeBlink::opening,
        5 SECONDS,
        20 SECONDS,
        40 MILLIS,
        200 MILLIS,
        SettingsManager::getSavedValue(SETTING_ENABLE_AUTO_BLINK)
    );

    sequencePlayer = new SequencePlayer(displayManager, displayTweenManager, eyeBlink, &Sequences::startup, SettingsManager::getSavedValue(SETTING_RARE_TRANSITION_CHANCE), true);
    
    sequencePlayer->addCommonTransitions({
        &Transitions::blink,
        //&Transitions::crossfade
    });

    sequencePlayer->addRareTransitions({
        &Transitions::drop,
        &Transitions::slide,
        &Transitions::losePower,
        &Transitions::glitch,
        &Transitions::explode,
        &Transitions::fizz,
        &Transitions::doomMelt,
        //&Transitions::earthquake,
        //&Transitions::expand,
        //&Transitions::shuffle,
        //&Transitions::pulse
        //&Transitions::modem
    });

    animatedMouth = new AnimatedMouth(displayManager, 33 MILLIS, 500, 500, SettingsManager::getSavedValue(SETTING_ENABLE_ANIMATED_MOUTH));

    /*
    boopSensor = new DigitalBoopSensor(
        displayManager, new OverlayPlayer(displayManager, &Overlays::boop, false, false), &Overlays::boop,
        &Transitions::glitch, PIN_BOOP_DIGITAL, BOOP_ACTIVATION_THRESHOLD, BOOP_ACTIVATION_MAX,
        EEPROM.read(EEPROM_BOOP_SENSOR)
    );
    */

    boopSensor = new AnalogBoopSensor(
        displayManager, new OverlayPlayer(displayManager, &Overlays::Boop::boop, false, false), 
        &Overlays::Boop::boop,  // enable overlay
        &Transitions::glitch,   // disable overlay
        Config::Pins::BOOP_ANALOG, Config::BOOP_TRIGGER_COUNT, Config::BOOP_TRIGGERS_MAX, 
        ((f32)SettingsManager::getSavedValue(SETTING_BOOP_TRIGGER_MULTIPLIER)) / 255.0f,  // trigger multiplier
        200,   // calibration readings
        false, // invert
        SettingsManager::getSavedValue(SETTING_ENABLE_BOOP_SENSOR)   // enable
    );

    fanControl = new PWMFan(Config::Pins::FAN_PWM, Config::FAN_PWM_CHANNEL, SettingsManager::getSavedValue(SETTING_FAN_PWM_SPEED));

    uptimeCounter = new UptimeCounter();

    settingsManager = new SettingsManager();
    createSettingsMenu();
    if(!settingsManager->init()) ESP.restart();

    controller = new StartupController(displayManager, 3 SECONDS, [](){changeController(new SettingsController(displayManager, settingsManager));});

    xTaskCreatePinnedToCore(loopAsync, "loopAsync", 2048, NULL, 1, &asyncLoopTask, 0);
}

// Main program loop
void loop(){
    // calculate delta time
    deltaTime = (Timestamp)(micros() - lastTime);
    if(deltaTime < Config::FRAMETIME) return;       // intentionally limiting updates per second to avoid fixed point imprecision issues
    lastTime = micros();

    #ifdef BT_GAMEPAD
    Gamepad::update();
    #endif
    inputHandler.update();
    sequencePlayer->update();
    controller->update();
    displayTweenManager->update();
    boopSensor->update();
    eyeBlink->update();
    ledStrip->update();
    uptimeCounter->update();
    animatedMouth->update();
    displayManager->update();
}

// Components to be updated independently on core 0
void loopAsync(void* pvParameters) {
    while(true) {
        hud->update();
    }
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

#ifdef BT_GAMEPAD
void createButtonMapping() {
    inputHandler.mapButtonsToJoystick(BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, handleJoystickInput);
    inputHandler.mapButtonsToJoystick(BTN_ALT_UP, BTN_ALT_DOWN, BTN_ALT_LEFT, BTN_ALT_RIGHT, handleJoystickInput);
    inputHandler.mapButtonsToJoystick(BTN_ALT_X, BTN_ALT_B, BTN_ALT_A, BTN_ALT_Y, handleJoystickInput);

    inputHandler.mapButtons({
        {BTN_TRIG1, BTN_STATE_PRESSED, [](){ 
            eyeBlink->handleInput(true); 
        }},

        {BTN_TRIG1, BTN_STATE_RELEASED, [](){ 
            eyeBlink->handleInput(false); 
        }},

        {BTN_X, BTN_STATE_PRESSED, [](){
            changeController( new FaceSwitcherController( displayManager, sequencePlayer, &Sequences::joyBlush, &Sequences::spooked, &Sequences::neutral, &Sequences::blushing, &Sequences::joy, &Sequences::angry, &Sequences::angryHappy, &Sequences::annoyed ));
        }},
        
        {BTN_SELECT, BTN_STATE_PRESSED, [](){ 
            changeController( new FaceSwitcherController( displayManager, sequencePlayer, &Sequences::heartEyes, &Sequences::questioning, &Sequences::dizzy, &Sequences::exclamation, &Sequences::dead, &Sequences::squinting, &Sequences::powerOff, &Sequences::wink ));
        }},

        {BTN_START, BTN_STATE_PRESSED, [](){ 
            changeController( new FaceSwitcherController( displayManager, sequencePlayer, &Sequences::crying, &Sequences::batteryCheck, &Sequences::uwu, &Sequences::randomize, &Sequences::nope, &Sequences::powerOff, &Sequences::owo, &Sequences::dizzy ));
        }},

        {BTN_A, BTN_STATE_PRESSED, [](){ 
            boopSensor->calibrate(); 
        }},

        {BTN_B, BTN_STATE_PRESSED, [](){ 
            changeController(new SettingsController(displayManager, settingsManager)); 
        }},

        {BTN_Y, BTN_STATE_PRESSED, [](){
            changeController( new AutomaticFaceSwitcherController(
                displayManager, sequencePlayer, 5 SECONDS, 20 SECONDS, {
                    &Sequences::neutral,
                    &Sequences::joy,
                    &Sequences::joyBlush,
                    &Sequences::blushing,
                    &Sequences::angryHappy,
                    &Sequences::angry,
                    &Sequences::annoyed,
                    &Sequences::spooked,
                    &Sequences::questioning,
                    &Sequences::exclamation,
                    &Sequences::dead,
                    &Sequences::squinting,
                    &Sequences::dizzy,
                    &Sequences::heartEyes,
                    &Sequences::powerOff,
                    &Sequences::uwu,
                    &Sequences::owo,
                    &Sequences::crying,
                })
            );
        }},
    });
}
#endif

void createSettingsMenu() {
    settingsManager->addSettings({
        new LambdaSetting(
            "Matrix",       // category
            "Brightness",   // name
            SETTING_MATRIX_BRIGHTNESS,  // unique key defined in Settings/SettingKeys.h enum SettingKey
            SETTING_TYPE_PERCENTAGE,    // setting type, as defined in Settings/Setting.h enum SettingType
            8, 0, 15, 1,        // default, minimum, maximum, increment values
            []() -> u8 { return displayManager->getMatrixBrightness(); },       // getter returning u8
            [](u8 value) { displayManager->setMatrixBrightness(value); }        // setter using u8
        ),

        new ComponentToggleSetting("Matrix", "Enable", SETTING_ENABLE_MATRIX, true, displayManager),        // category, name, unique key, default value, child of ToasterComponent

        new ComponentToggleSetting("LED Strip", "Enable", SETTING_ENABLE_LEDSTRIP, true, ledStrip),

        new ComponentToggleSetting("Auto blink", "Enable", SETTING_ENABLE_AUTO_BLINK, true, eyeBlink),

        new ComponentToggleSetting("Boop sensor", "Enable", SETTING_ENABLE_BOOP_SENSOR, false, boopSensor),

        new LambdaSetting(
            "Boop sensor",
            "Sensitivity",
            SETTING_BOOP_TRIGGER_MULTIPLIER,
            SETTING_TYPE_PERCENTAGE,
            127, 0, 255, 17,
            []() -> u8 { return boopSensor->getTriggerMultiplier(); }, 
            [](u8 value) { boopSensor->setTriggerMultiplier(value); }
        ),

        new ActionSetting("Boop sensor", "Calibrate", [](u8 value) { boopSensor->calibrate(); }),       // category, name, function to be called

        new ComponentToggleSetting("Mouth anim.", "Enable", SETTING_ENABLE_ANIMATED_MOUTH, false, animatedMouth),

        new ActionSetting("Mouth anim.", "Calibrate", [](u8 value) { animatedMouth->reset(); }),

        new LambdaSetting(
            "Rare transit.",
            "Chance",
            SETTING_RARE_TRANSITION_CHANCE,
            SETTING_TYPE_PERCENTAGE,
            25, 0, 255, 17,
            []() -> u8 { return sequencePlayer->getRareTransitionChance(); }, 
            [](u8 value) { sequencePlayer->setRareTransitionChance(value); }
        ),

        new LambdaSetting(
            "Fan speed",
            "",
            SETTING_FAN_PWM_SPEED,
            SETTING_TYPE_PERCENTAGE,
            255, 0, 255, 51,
            []() -> u8 { return fanControl->getSpeed(); }, 
            [](u8 value) { fanControl->setSpeed(value); }
        ),

        new ActionSetting(
            "Misc.",
            "Snake game",
            [](u8 value) { 
                animatedMouth->stop(); 
                changeController(new SnakeGameController(displayManager, MOUTH, EYES, 200 MILLIS)); 
            }
        ),

        /*
        new ActionSetting(
            "Misc.",
            "Analyzer",
            [](u8 value) { 
                animatedMouth->stop(); 
                changeController(new SpectrumAnalyzerController(displayManager)); 
            }
        ),
        */

        // new ActionSetting("Misc.", "Strobe test", [](u8 value) { changeController(new StrobeController(displayManager, sequencePlayer, displayTweenManager)); }),

        new ActionSetting("System", "Restart", [](u8 value) { ESP.restart(); }),

        new ActionSetting("System", "Save settings", [](u8 value) { settingsManager->saveSettings(); }),
    });
}

ButtonDebounce debounceX, debounceY;
u8 prevX, prevY;
// filters out raw joystick data and sends to the active controller
void handleJoystickInput(u8 x, u8 y) {
    // round and debounce
    u8 rX = roundAnalogValue(x);
    u8 rY = roundAnalogValue(y);

    if (rX == 0x80) debounceX.reset();
    else if (debounceX.shouldActivate()) controller->handleInput(rX, 0x80);

    if (rY == 0x80) debounceY.reset();
    else if (debounceY.shouldActivate()) controller->handleInput(0x80, rY);

    // send raw joystick data to the active controller if needed
    if (prevX != x || prevY != y) {
        controller->handleRawInput(x, y);
    }

    prevX = x;
    prevY = y;
}

void safeMode() {
    if (controller->getType() != CONTROLLER_SETTINGS) {
        changeController(new SettingsController(displayManager, settingsManager));
    }
    Sequence* s = sequencePlayer->getSequence();
    if (s != &Sequences::neutral && s != &Sequences::startup) {
        sequencePlayer->transitionSequence(&Sequences::neutral);
    }
}