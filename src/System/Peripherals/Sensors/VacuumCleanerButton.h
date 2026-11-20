#pragma once

/**
 * Vacuum cleaner button (pullup) for powerstick synth
 *
 * Custom overrides for sensor events, plays sequences and sound effects on activation
 */

#include "Sensor.h"
#include "DFRobotDFPlayerMini.h"
#include "HardwareSerial.h"
#include "Assets/Sequences/Powerstick.h"

class VacuumCleanerButton : public Sensor {
  private:
    DisplayManager* displayManager;
    SequencePlayer* sequencePlayer;
	HardwareSerial* serial;
    DFRobotDFPlayerMini* mp3player;
	Timestamp lastStartTime;
	bool initSuccess;
	bool buttonTapped;

	void fail() {
		sequencePlayer->transitionSequence(sequencePlayer->getSequence() == &Sequences::batteryCheck ? &Sequences::powerstickDefault : &Sequences::batteryCheck);
	}

	void playMp3(int fileNumber) {
		if (!initSuccess) return;
		mp3player->play(fileNumber);
	}

	void stopMp3() {
		if (!initSuccess) return;
		mp3player->stop();
	}

  protected:
    void onEnable() override {
		//(if (!initSuccess) {
		if (false) {
			fail();
			return;
		}

		lastStartTime = millis();
		playMp3(2);		// 0002_vacuumStart
        sequencePlayer->transitionSequence(&Sequences::powerstickVacuumStart);
    }
	
    void onDisable() override {
		//if (!initSuccess) {
		if (false) {
			fail();
			return;
		}

		long heldTime = millis() - lastStartTime;

		if (heldTime > 200) buttonTapped = false;

		// animations
		if (heldTime > 4000) {
			sequencePlayer->transitionSequence(&Sequences::powerstickVacuumEnd4000);
			playMp3(6);		// 0006_vacuumEnd4000
		} 
		else if (heldTime > 2000) {
			sequencePlayer->transitionSequence(&Sequences::powerstickVacuumEnd2000);
			playMp3(5);		// 0005_vacuumEnd2000
		} 
		else if (heldTime > 1500) {
			sequencePlayer->transitionSequence(&Sequences::powerstickVacuumEnd1500);
			playMp3(4);		// 0004_vacuumEnd1500
		} 
		else if (heldTime > 1000) {
			sequencePlayer->transitionSequence(&Sequences::powerstickVacuumEnd1000);
			playMp3(3);		// 0003_vacuumEnd1000
		} 
		else if (heldTime < 100) {
			u8 brightness = displayManager->getMatrixBrightness() + 5;
			brightness = brightness > 15 ? 0 : brightness;
			displayManager->setMatrixBrightness(brightness);		// brightness adjustment for double tap
			sequencePlayer->transitionSequence(&Sequences::powerstickActivate);
			playMp3(8);		// 0008_activate
		}
		else {
			sequencePlayer->transitionSequence(&Sequences::powerstickVacuumFail);
			playMp3(1);		// 0001_fail
		}
    }
    
    void onUpdate() override {
    }

  public:
    VacuumCleanerButton(u8 sensorPin, DisplayManager* displayManager, SequencePlayer* sequencePlayer, bool inverted = false, bool enabled = true)
        : Sensor(sensorPin, 1, 4, inverted, enabled)
		, displayManager(displayManager)
 		, sequencePlayer(sequencePlayer)
		, buttonTapped(false)
    {
        pinMode(sensorPin, INPUT_PULLUP);
		serial = new HardwareSerial(1);
		serial->begin(9600, SERIAL_8N1, 9, 7);
        mp3player = new DFRobotDFPlayerMini();
		if (mp3player->begin(*serial)) {
			TOASTER_LOG("Vacuum cleaner button initialized successfully");
			mp3player->volume(20);
			playMp3(7);		// 0007_startup
			initSuccess = true;
		} else {
			TOASTER_LOG("Vacuum cleaner button init failed");
			initSuccess = false;
		}
    }
};