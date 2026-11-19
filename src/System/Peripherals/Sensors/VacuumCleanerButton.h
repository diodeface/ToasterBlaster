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
    SequencePlayer* sequencePlayer;
	HardwareSerial* serial;
    DFRobotDFPlayerMini* mp3player;
	Timestamp lastStartTime;
	bool initSuccess;

	void fail() {
		sequencePlayer->transitionSequence(sequencePlayer->getSequence() == &Sequences::batteryCheck ? &Sequences::powerstickDefault : &Sequences::batteryCheck);
	}

  protected:
    void onEnable() override {
		if (!initSuccess) {
			fail();
			return;
		}

		lastStartTime = millis();
		mp3player->play(2);		// 0002_vacuumStart
        sequencePlayer->transitionSequence(&Sequences::powerstickStartVacuuming);
    }
	
    void onDisable() override {
		if (!initSuccess) {
			fail();
			return;
		}

		long heldTime = millis() - lastStartTime;

		if (heldTime > 4000) {
			sequencePlayer->transitionSequence(&Sequences::powerstickEndVacuuming);
			mp3player->play(6);		// 0006_vacuumEnd4000
		} 
		else if (heldTime > 2000) {
			sequencePlayer->transitionSequence(&Sequences::powerstickEndVacuuming);
			mp3player->play(5);		// 0005_vacuumEnd2000
		} 
		else if (heldTime > 1500) {
			sequencePlayer->transitionSequence(&Sequences::powerstickDefault);
			mp3player->play(4);		// 0004_vacuumEnd1500
		} 
		else if (heldTime > 1000) {
			sequencePlayer->transitionSequence(&Sequences::powerstickDefault);
			mp3player->play(3);		// 0003_vacuumEnd1000
		} 
		else {
			sequencePlayer->transitionSequence(&Sequences::powerstickError);
			mp3player->play(1);		// 0001_fail
		}  
    }
    
    void onUpdate() override {
    }

  public:
    VacuumCleanerButton(u8 sensorPin, SequencePlayer* sequencePlayer, bool inverted = false, bool enabled = true)
        : Sensor(sensorPin, 1, 4, inverted, enabled)
		, sequencePlayer(sequencePlayer)
    {
        pinMode(sensorPin, INPUT_PULLUP);
		serial = new HardwareSerial(1);
		serial->begin(9600, SERIAL_8N1, 9, 7);
        mp3player = new DFRobotDFPlayerMini();
		if (mp3player->begin(*serial)) {
			TOASTER_LOG("Vacuum cleaner button initialized successfully");
			mp3player->volume(20);
			initSuccess = true;
		} else {
			TOASTER_LOG("Vacuum cleaner button init failed");
			initSuccess = false;
		}
    }
};