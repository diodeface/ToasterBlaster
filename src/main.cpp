/**
 * Adeon's Toaster Blaster
 * ---------------------------------------------------------------------------
 * This is a LED control program for protogen helmets to be used with MAX7219
 * LED matrixes and ESP32 dev boards. Keyframe animation system allows you
 * to create animations with ease. Facial expressions are controlled with an
 * inexpensive bluetooth controller (MOCUTE-052F), with more gamepads to be
 * supported in the future.
 * 
 * This is a pre-release with a some bugs, unfinished code and 
 * lacking documentation. Use at your own risk.
 */

#include <Arduino.h>
#include "main.h"

DisplayManager* displayManager;
SequencePlayer* sequencePlayer;
Controller* controller;
TaskHandle_t asyncLoopTask;
Timestamp lastTime;
#ifdef ENABLE_ANIMATED_MOUTH
AnimatedMouth* animatedMouth;
#endif

SettingsManager* settingsManager;
HeadsUpDisplay* hud;
TweenManager* tweenManager;
AnalogBoopSensor* boopSensor;
EyeBlink* eyeBlink;
LEDStrip* ledStrip;
UptimeCounter* uptimeCounter;
PWMFan* fanControl;

void setup() {
    TOASTER_LOG("\n\n\n\nStarting up...\n");
    randomSeed(analogRead(0));
    EEPROM.begin(16);

    hud = new U8G2HUDWrapper(Config::Pins::HUD_SCL, Config::Pins::HUD_SDA);

    displayManager = new DisplayManager(
        new Max7219(Config::Pins::MATRIX_DIN, Config::Pins::MATRIX_CS, Config::Pins::MATRIX_CLK), hud, EEPROM.read(Config::EEPROM::MATRIX)
    );

    tweenManager = new TweenManager(displayManager);

    // Register displays (matrix position, number of rows (size), bitmask, hud preview position)
    // Simply comment out any displays that are not in use.
    using namespace Config::Displays;
    displayManager->addDisplays({
        new Display(POS_EYE_L,    16,   EYE_L,    {49, 0},    tweenManager),
        new Display(POS_EYE_R,    16,   EYE_R,    {1, 0},     tweenManager),
        new Display(POS_MOUTH_L,  32,   MOUTH_L,  {34, 8},    tweenManager),
        new Display(POS_MOUTH_R,  32,   MOUTH_R,  {0, 8},     tweenManager),
        new Display(POS_NOSE_L,   8,    NOSE_L,   {35, 0},    tweenManager),
        new Display(POS_NOSE_R,   8,    NOSE_R,   {23, 0},    tweenManager),
    });

    // To add persistent rotation or other effects to your displays, add a global effect:
    #if DISPLAY_SET == 1
    displayManager->addGlobalEffect(new Rotate180(ALL));
    #endif

    ledStrip = new LEDStrip(EEPROM.read(Config::EEPROM::LEDSTRIP));

    #ifdef BT_CONTROLLER
    BLEGamepad::init();
    createButtonMapping();
    #endif

    eyeBlink = new EyeBlink(
        displayManager,
        &Overlays::EyeBlink::closing,
        &Overlays::EyeBlink::closed,
        &Overlays::EyeBlink::opening,
        5 SECONDS,
        20 SECONDS,
        EEPROM.read(Config::EEPROM::AUTO_BLINK)
    );

    sequencePlayer = new SequencePlayer(displayManager, tweenManager, ledStrip, eyeBlink, &Sequences::startup, EEPROM.read(Config::EEPROM::RARE_TRANSITION_CHANCE), true);
    
    sequencePlayer->addCommonTransitions({
        &Transitions::blink,
        &Transitions::crossfade
    });

    sequencePlayer->addRareTransitions({
        &Transitions::drop,
        &Transitions::slide,
        &Transitions::losePower,
        &Transitions::glitch,
        //&Transitions::expand,  // having all the LEDs on may cause a voltage drop and crash the ESP32
        &Transitions::explode,
        //&Transitions::shuffle,
        &Transitions::fizz,
        &Transitions::doomMelt,
    });

    #ifdef ENABLE_ANIMATED_MOUTH
    animatedMouth = new AnimatedMouth(displayManager, 33 MILLIS, 500, 500, EEPROM.read(Config::EEPROM::ANIMATED_MOUTH));
    #endif

    /*
    boopSensor = new DigitalBoopSensor(
        displayManager, new OverlayPlayer(displayManager, &Overlays::boop, false, false), &Overlays::boop,
        &Transitions::glitch, PIN_BOOP_DIGITAL, BOOP_ACTIVATION_THRESHOLD, BOOP_ACTIVATION_MAX,
        EEPROM.read(EEPROM_BOOP_SENSOR)
    );
    */

    boopSensor = new AnalogBoopSensor(
        displayManager, new OverlayPlayer(displayManager, &Overlays::boop, false, false), 
        &Overlays::boop,        // enable overlay
        &Transitions::glitch,   // disable overlay
        Config::Pins::BOOP_ANALOG, Config::BOOP_TRIGGER_COUNT, Config::BOOP_TRIGGERS_MAX, 
        (f32)EEPROM.read(Config::EEPROM::BOOP_TRIGGER_MULTIPLIER) * 0.01f,  // trigger multiplier
        200,   // calibration readings
        false, // invert
        EEPROM.read(Config::EEPROM::BOOP_SENSOR)   // enable
    );

    fanControl = new PWMFan(Config::Pins::FAN_PWM, Config::FAN_PWM_CHANNEL, EEPROM.read(Config::EEPROM::FAN_PWM_SPEED));

    createSettingsMenu();

    controller = new StartupController(displayManager, 4 SECONDS, [](){changeController(new SettingsController(displayManager, settingsManager));});
    
    uptimeCounter = new UptimeCounter();

    xTaskCreatePinnedToCore(loopAsync, "loopAsync", 2048, NULL, 1, &asyncLoopTask, 1);
}

// Main program loop
void loop(){
    // calculate delta time
    deltaTime = (Timestamp)(micros() - lastTime);
    if(deltaTime < Config::FRAMETIME) return;
    lastTime = micros();

    #ifdef BT_CONTROLLER
    BLEGamepad::update();
    #endif
    sequencePlayer->update();
    controller->update();
    tweenManager->update();
    boopSensor->update();
    eyeBlink->update();
    ledStrip->update();
    uptimeCounter->update();
    #ifdef ENABLE_ANIMATED_MOUTH
    animatedMouth->update();
    #endif
    displayManager->update();
}

// Components to be updated independently on core 1
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

void handleJoystickInput(u8 x, u8 y) {
    BT_LOG("Joystick input (%d, %d)\n", x, y);
    controller->handleInput(x, y);
}

void createButtonMapping() {
    BLEGamepad::inputHandler.mapButtonsToJoystick(BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, handleJoystickInput);
    BLEGamepad::inputHandler.mapButtonsToJoystick(BTN_ALT_UP, BTN_ALT_DOWN, BTN_ALT_LEFT, BTN_ALT_RIGHT, handleJoystickInput);
    BLEGamepad::inputHandler.mapButtonsToJoystick(BTN_ALT_X, BTN_ALT_B, BTN_ALT_A, BTN_ALT_Y, handleJoystickInput);

    BLEGamepad::inputHandler.mapButtons({
        {BTN_TRIG1, BTN_STATE_PRESSED, [](){ 
            eyeBlink->handleInput(true); 
        }},

        {BTN_TRIG1, BTN_STATE_RELEASED, [](){ 
            eyeBlink->handleInput(false); 
        }},
        
        {BTN_SELECT, BTN_STATE_PRESSED, [](){ 
            changeController( new FaceSwitcherController( displayManager, sequencePlayer, &Sequences::heartEyes, &Sequences::dead, &Sequences::dizzy, &Sequences::questioning ));
        }},

        {BTN_START, BTN_STATE_PRESSED, [](){ 
            changeController( new FaceSwitcherController( displayManager, sequencePlayer, &Sequences::angry, &Sequences::annoyed, &Sequences::spooked, &Sequences::blushing ));
        }},

        {BTN_X, BTN_STATE_PRESSED, [](){
            changeController( new FaceSwitcherController( displayManager, sequencePlayer, &Sequences::neutral, &Sequences::joy, &Sequences::squinting, &Sequences::angryHappy ));
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
                    &Sequences::squinting,
                    &Sequences::angryHappy,
                    &Sequences::spooked,
                    &Sequences::angry,
                    &Sequences::blushing
                })
            );
        }},
    });
}

void createSettingsMenu() {
    settingsManager = new SettingsManager();

    settingsManager->addSettings({
        new LambdaSetting(
            "Matrix", "Brightness", 15, 
            []() -> u8 { return displayManager->getMatrixBrightness(); },
            [](i8 value) { displayManager->addMatrixBrightness(value); }, 
            Config::EEPROM::MATRIX_BRIGHTNESS
        ),

        new ComponentToggleSetting("Matrix", "Enable", displayManager, Config::EEPROM::MATRIX),

        new ComponentToggleSetting("LED Strip", "Enable", ledStrip, Config::EEPROM::LEDSTRIP),

        new ComponentToggleSetting("Auto blink", "Enable", eyeBlink, Config::EEPROM::AUTO_BLINK),

        new ComponentToggleSetting("Boop sensor", "Enable", boopSensor, Config::EEPROM::BOOP_SENSOR),
        
        new LambdaSetting(
            "Boop sensor", "Sensitivity", 100,
            []() -> u8 { return boopSensor->getTriggerMultiplier(); }, 
            [](i8 value) { boopSensor->addTriggerMultiplier(value); },
            Config::EEPROM::BOOP_TRIGGER_MULTIPLIER
        ),

        new LambdaSetting(
            "Boop sensor", "Calibrate", 0, // add function only
            []() -> u8 { return 0; }, 
            [](i8 value) { boopSensor->calibrate(); }
        ),

        #ifdef ENABLE_ANIMATED_MOUTH
        new ComponentToggleSetting("Mouth anim.", "Enable", animatedMouth, Config::EEPROM::ANIMATED_MOUTH),

        new LambdaSetting(
            "Mouth anim.", "Noise floor", 250,
            []() -> u8 { return animatedMouth->getNoiseFloor(); }, 
            [](i8 value) { animatedMouth->addNoiseFloor(value * 10); },
            Config::EEPROM::ANIMATED_MOUTH_NOISE_FLOOR
        ),

        new LambdaSetting(
            "Mouth anim.", "Peak minimum", 250,
            []() -> u8 { return animatedMouth->getPeakMinimum(); }, 
            [](i8 value) { animatedMouth->addPeakMinimum(value * 10); },
            Config::EEPROM::ANIMATED_MOUTH_PEAK_MIN
        ),

        new LambdaSetting(
            "Mouth anim.", "Calibrate mic.", 0,
            []() -> u8 { return 0; }, 
            [](i8 value) { animatedMouth->reset(); }
        ),
        #endif

        new LambdaSetting(
            "Transitions", "Rare chance %", 100,
            []() -> u8 { return sequencePlayer->getRareTransitionChance(); }, 
            [](i8 value) { sequencePlayer->addRareTransitionChance(value * 10); },
            Config::EEPROM::RARE_TRANSITION_CHANCE
        ),

        new LambdaSetting(
            "Fan control", "Speed", 255,
            []() -> u8 { return fanControl->getSpeed(); }, 
            [](i8 value) { fanControl->addSpeed(value * 10); },
            Config::EEPROM::FAN_PWM_SPEED
        ),

        new LambdaSetting(
            "Fun", "Snake Game", 0,
            []() -> u8 { return 0; }, 
            [](i8 value) { 
                #ifdef ENABLE_ANIMATED_MOUTH
                animatedMouth->stop(); 
                #endif
                changeController(new SnakeGameController(displayManager, MOUTH, EYES, 200 MILLIS)); 
            }
        ),

        new LambdaSetting(
            "Fun", "Analyzer", 0,
            []() -> u8 { return 0; }, 
            [](i8 value) { 
                #ifdef ENABLE_ANIMATED_MOUTH
                animatedMouth->stop(); 
                #endif
                changeController(new SpectrumAnalyzerController(displayManager)); 
            }
        ),

        new LambdaSetting(
            "Misc.", "Restart", 0,
            []() -> u8 { return 0; }, 
            [](i8 value) { 
                ESP.restart();
            }
        ),

        new LambdaSetting(
            "Misc.", "Save settings", 0, 
            []() -> u8 { return 0; }, 
            [](i8 value) { EEPROM.commit(); }
        ),
    });
}