#pragma once

#include <Arduino.h>
#include "Utilities/Typedefs.h"
#include "Utilities/Util.h"
#include "Components/ToasterComponent.h"
#include "Drawing/DisplayManager.h"
#include "Animation/Sequence/Sequences.h"
#include "Drawing/Effects/MouthEffect.h"
#include "Utilities/Globals.h"

/**
 * Animates mouth according to microphone input.
 */
class AnimatedMouth : public ToasterComponent {
  private:
    const u8 overlayId = 0;
    bool updated;

    // microphone parameters
    const u32 PEAK_FALLOFF = 50;        // peak falloff rate
    const f32 FALLOFF = 0.95f;          // reading falloff rate
    const u32 BASELINE_SAMPLES = 256;   // amount of samples made during initialization
    
    // adjustable microphone parameters
    u32 peakMinimum = 500;           // lowest peak value
    u32 noiseFloor = 500;        // how much noise is filtered

    i32 baseline;
    i32 peak;
    i32 result;

    u8 barHeights[32];

    const f32 SINE_AMPLITUDE = 1.03f;
    f32 precomputedLog[32];
    f32 precomputedPow[32];

    Timestamp time, period;

    DisplayManager *displayManager;
    Effect *mouthEffect, *mouthEffectReverse;

    void calibrate() {
        // todo: rewrite this thing to do calibration without interrupting the program like in AnalogBoopSensor
        std::fill(barHeights, barHeights + 32, 0);

        baseline = 0;
        result = 0;
        peak = peakMinimum;

        // todo: move this to a function so that calibration can be run at any time
        TOASTER_LOG("Collecting %d samples for AnimatedMouth...\n", BASELINE_SAMPLES);

        // calculate baseline value for mic (slow)
        for(i32 i = 0; i < BASELINE_SAMPLES; i++) {
            baseline += analogRead(Config::Pins::MIC);
            delay(1);
        }
        baseline /= BASELINE_SAMPLES;
    }

    void precompute() {
        for (u8 i = 0; i < 32; i++) {
            precomputedLog[i] = log2(1 + i * 2);
            precomputedPow[i] = pow(SINE_AMPLITUDE, i);
        }
    }

    void init() {
        enabled = true;
        time = 0;

        displayManager->removeOverlay(overlayId);
        displayManager->addOverlay(mouthEffect, overlayId);
        displayManager->addOverlay(mouthEffectReverse, overlayId);
    }

  public:
    AnimatedMouth(
        DisplayManager *displayManager, 
        Timestamp period = 33 MILLIS, 
        u32 peakMinimum = 500,
        u32 noiseFloor = 500,
        bool enabled = true
    ) 
        : peakMinimum(peakMinimum)
        , noiseFloor(noiseFloor)
    {
        this->displayManager = displayManager;
        this->enabled = enabled;
        this->period = period;
        this->updated = false;
        time = 0;

        mouthEffect = new MouthEffect(MOUTH_R, barHeights, true);
        mouthEffectReverse = new MouthEffect(MOUTH_L, barHeights);

        precompute();
        calibrate();
        if(enabled) init();
        TOASTER_LOG("AnimatedMouth Initialized.\n");
    }

    void reset() override {
        calibrate();
    }

    void start() override {
        init();
    }

    void stop() override {
        enabled = false;
        displayManager->removeOverlay(overlayId);
        displayManager->requestRedraw();
    }

    void pause() override {
        enabled = false;
    }

    void update() override {
        if(!enabled) return;
        if((time += deltaTime) < period) return;
        time = 0;

        // capture reading
        i32 rawValue = analogRead(Config::Pins::MIC);
        i32 reading = abs(rawValue - baseline) - noiseFloor;
        if(reading < 0) reading = 0;
        result = max(reading, (i32)(result * FALLOFF));
        MIC_LOG("reading: %5d, difference: %5d, result: %5d\n", rawValue, reading, result);

        // calculate new peak
        if(result > peak) peak = result;
        micValue = result;
        if(peak > peakMinimum) peak -= PEAK_FALLOFF;
        if(peak < peakMinimum) peak = peakMinimum;
        
        // calculate values for mouth drawing
        f32 normalizedResult = (f32)result / (f32)peak;
        //f32 normalizedResult = 1.0f;
        f32 sineFrequency = 0.4 * max(normalizedResult, 0.5f);
        f32 phase = millis() * 0.025f;

        bool requestRedraw = false;
        // do the math
        for(u8 i = 0; i < 32; i++) {
            f32 roundedResult = nearbyint((precomputedLog[i] + sin(i * sineFrequency + phase) * precomputedPow[i]) * normalizedResult);
            barHeights[i] = roundedResult < 0 ? 0 : roundedResult;
            if(barHeights[i] > 0) {
                requestRedraw = true;
                updated = true;
            }       
        }

        if(requestRedraw) {
            displayManager->requestRedraw();
        } else if(updated) {
            displayManager->requestRedraw();
            updated = false;
        }
    }

    u32 getNoiseFloor() { return noiseFloor; }
    u32 getPeakMinimum() { return peakMinimum; }
    
    void addNoiseFloor(i32 amount) { 
        u32 newValue = noiseFloor + amount;
        if( newValue < 500 || newValue > 2500) return;
        noiseFloor = newValue;
    }

    void addPeakMinimum(i32 amount) { 
        u32 newValue = peakMinimum + amount;
        if( newValue < 500 || newValue > 2500) return;
        peakMinimum = newValue;
    }
};