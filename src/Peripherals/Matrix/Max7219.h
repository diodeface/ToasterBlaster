#pragma once

#include "Utilities/Typedefs.h"
#include "Matrix.h"
#include <Arduino.h>
#include <SPI.h>

#define NUM_DISPLAYS 14

/**
 * Basic Max7219 display driver.
 */
class Max7219 : public Matrix {
  private:
    u8 DIN, CS, CLK;

    void transferAll(u8 address, u8 value) {
        for(size_t i = 0; i < NUM_DISPLAYS; i++) {
            digitalWrite(CS, LOW);
            SPI.transfer(address);
            SPI.transfer(value);
            digitalWrite(CS, HIGH);
        }
    }

    void initialize() {
        transferAll(0x0b, 0x07);  // scan limit
        transferAll(0x09, 0x00);  // decode mode
        transferAll(0x0c, 0x01);  // shutdown
        transferAll(0x0f, 0x00);  // display test
    }

    void clear() {
        for(size_t i = 0; i < 8; i++) transferAll(i + 1, 0);
    }

  public:
    Max7219(u8 DIN, u8 CS, u8 CLK, u8 brightness = 0) : Matrix(brightness) {
        this->DIN = DIN;
        this->CS = CS;
        this->CLK = CLK;
        pinMode(CS, OUTPUT);

        SPI.setBitOrder(MSBFIRST);
        SPI.setClockDivider(SPI_CLOCK_DIV2);
        SPI.begin(CLK, -1, DIN, CS);

        // initialization sometimes fails with many chained displays
        // that's why we brute force it here
        for(u8 i = 0; i < 20; i++) {
            initialize();
        }

        clear();
    }

    void init() override {
        initialize();
    }

    void setBrightness(u8 brightness) override {
        this->brightness = brightness & 0xf;
        transferAll(0x0a, this->brightness);
    }

    void drawColumn(u8 col, u8 value) override {
        i32 n = col / 8;
        i32 c = col % 8;
        digitalWrite(CS, LOW);
        for(size_t i = 0; i < NUM_DISPLAYS; i++) {
            if(i == n) {
                SPI.transfer(c + 1);
                SPI.transfer(value);
            } else {
                SPI.transfer(0);
                SPI.transfer(0);
            }
        }
        digitalWrite(CS, HIGH);
    }

    void drawBitmap(const u8* bitmap, size_t size, u8 position) override {
        for(i32 i = 0; i < size; i++) {
            i32 col = position + i;
            if(col >= 0 && col < 112) drawColumn(col, bitmap[i]);
        }
    }
};