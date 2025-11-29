#pragma once

// todo: finish this thing

#include <driver/i2s.h>
#include <arduinoFFT.h>
#include "Arduino.h"
#include "Typedefs.h"

#include "System/Utilities/Globals.h"
#include "System/Drawing/DisplayManager.h"
#include "System/Drawing/Effects/Clear.h"
#include "System/Drawing/Effects/Rectangle.h"
#include "System/Drawing/Effects/SetPixel.h"

#include "Controller.h"

#define SAMPLES 512
#define SAMPLING_FREQ 44100
#define NOISE 1000
#define BANDFALLOFF 1
#define MAXPEAKGRAVITY 0.5f

/**
 * Spectrum Analyzer Controller (super work in progress)
 * this thing sometimes crashes on the i2s setup stage and i haven't yet figured out why
 * 
 * Draws a Spectrum analyzer.
 * 
 * Most of the code originates from this article by John Bradnam:
 * https://www.hackster.io/john-bradnam/3d-printed-fft-spectrum-analyzer-5dab94
 */

using namespace Config::Displays;

class SpectrumAnalyzerController : public Controller {
  private:
    DisplayManager* displayManager;

    int16_t samples[SAMPLES];
    double vReal[SAMPLES];
    double vImag[SAMPLES];

    int bandValues[16];
    uint8_t barValues[16];
    f32 peakGravity[16];
    f32 peakValues[16];

    arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

  public:
    SpectrumAnalyzerController(DisplayManager* displayManager)
        : Controller(CONTROLLER_SPECTRUM_ANALYZER, SP_STOP)
        , displayManager(displayManager) {
        displayManager->hud->drawStatusText("Analyzer");
        
        for (Display* d : *displayManager->getDisplays(ALL)){
            Clear::clear(d->bitmapBuffer, d->getSize());
        }

        // setup i2s
        const i2s_config_t i2s_config = {
            .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
            .sample_rate = SAMPLING_FREQ,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = I2S_COMM_FORMAT_I2S_MSB,
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Interrupt level 1, default 0
            .dma_buf_count = 4,
            .dma_buf_len = SAMPLES,
            .use_apll = false,
        };
        adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_0db);
        i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
        i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);

        for(int i = 0; i < 16; i++) {
            peakValues[i] = 0.0f;
            peakGravity[i] = 0.0f;
            barValues[i] = 0;
        }
    }

    void update() override {
        // read bytes
        size_t bytes_read;
        i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytes_read, portMAX_DELAY);

        for(uint16_t i = 0; i < SAMPLES; i++) {
            vReal[i] = samples[i];
            vImag[i] =
                0.0;  // Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
        }

        // Compute FFT
        FFT.DCRemoval();
        FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        FFT.Compute(FFT_FORWARD);
        FFT.ComplexToMagnitude();

        // Reset bandValues[]
        for(int i = 0; i < 16; i++) {
            bandValues[i] = 0;
        }

        // Analyse FFT results
        for(int i = 2; i < (SAMPLES / 2); i++) {  // Don't use sample 0 and only first SAMPLES/2 are usable. Each array
                                                  // element represents a frequency bin and its value the amplitude.
            if(vReal[i] > NOISE) {                // Add a crude noise filter
                if(i <= 2) bandValues[0] += (int)vReal[i];
                if(i > 2 && i <= 3) bandValues[1] += (int)vReal[i];
                if(i > 3 && i <= 5) bandValues[2] += (int)vReal[i];
                if(i > 5 && i <= 7) bandValues[3] += (int)vReal[i];
                if(i > 7 && i <= 9) bandValues[4] += (int)vReal[i];
                if(i > 9 && i <= 13) bandValues[5] += (int)vReal[i];
                if(i > 13 && i <= 18) bandValues[6] += (int)vReal[i];
                if(i > 18 && i <= 25) bandValues[7] += (int)vReal[i];
                if(i > 25 && i <= 36) bandValues[8] += (int)vReal[i];
                if(i > 36 && i <= 50) bandValues[9] += (int)vReal[i];
                if(i > 50 && i <= 69) bandValues[10] += (int)vReal[i];
                if(i > 69 && i <= 97) bandValues[11] += (int)vReal[i];
                if(i > 97 && i <= 135) bandValues[12] += (int)vReal[i];
                if(i > 135 && i <= 189) bandValues[13] += (int)vReal[i];
                if(i > 189 && i <= 264) bandValues[14] += (int)vReal[i];
                if(i > 264) bandValues[15] += (int)vReal[i];
            }
        }

        for(int i = 0; i < 16; i++) {
            // calculate bar heights
            uint8_t newValue = map(bandValues[i], 0, 20000, 0, 8);
            barValues[i] = max(newValue > 8 ? 8 : newValue, barValues[i] - BANDFALLOFF);

            // calculate peak positions
            if(newValue > peakValues[i]) {
                peakGravity[i] = 0;
                peakValues[i] = barValues[i];
            } else {
                peakGravity[i] += peakGravity[i] <= MAXPEAKGRAVITY ? 0.01 : 0;
                peakValues[i] = peakValues <= 0 ? 0 : peakValues[i] - peakGravity[i];
            }
        }

        for (Display* d : *displayManager->getDisplays(MOUTH)){
            Clear::clear(d->bitmapBuffer, d->getSize());
            for(int i = 0; i < 16; i++) { 
                // draw bars
                if(barValues[i] > 0) {
                    Rectangle::rectangle(d->bitmapBuffer, d->getSize(), i * 2, 8 - barValues[i], 2, barValues[i], DRAW_MODE_1);
                }

                // draw peaks
                Rectangle::rectangle(d->bitmapBuffer, d->getSize(), i * 2, 7 - (u8)peakValues[i], 2, 1, DRAW_MODE_1);
            }
        }

        // draw on hud
        displayManager->hud->clearBottom();
        for(int i = 0; i < 16; i++) { 
            // draw bars
            if(barValues[i] > 0) {
                displayManager->hud->drawBox(i * 8, 64 - barValues[i] * 6, 6, barValues[i] * 6);
            }

            // draw peaks
            displayManager->hud->drawHLine(i * 8, 64 - (u8)peakValues[i] * 6, 6);
        }
        

        displayManager->requestRedraw();
    }

    void handleInput(u8 x, u8 y) override {
    }
};