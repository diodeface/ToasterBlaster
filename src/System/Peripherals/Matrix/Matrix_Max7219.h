#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "Typedefs.h"
#include "config.h"
#include "Matrix.h"

/**
 * Basic Max7219 display driver.
 */
class Matrix_Max7219 : public Matrix {
  private:
    u8 DIN, CS, CLK;

    inline void transfer(u8 address, u8 data) {
        SPI.transfer(address);
        SPI.transfer(data);
    }

    void transferAll(u8 address, u8 data) {
        digitalWrite(CS, LOW);
        for(size_t i = 0; i < Config::Displays::NUM_DISPLAYS; i++) transfer(address, data);
        digitalWrite(CS, HIGH);
    }

    inline void noop() {
        transfer(0x00, 0x00);
    }

    void initialize() {
        transferAll(0x0b, 0x07);  // scan limit
        transferAll(0x09, 0x00);  // decode mode
        transferAll(0x0c, 0x01);  // shutdown
        transferAll(0x0f, 0x00);  // display test
    }

  public:
    Matrix_Max7219(
        u8 DIN, 
        u8 CS, 
        u8 CLK, 
        u8 brightness = 0
    ) 
        : Matrix(brightness)
        , DIN(DIN)
        , CS(CS)
        , CLK(CLK) 
    {
        pinMode(CS, OUTPUT);

        SPI.setBitOrder(MSBFIRST);
        SPI.setClockDivider(SPI_CLOCK_DIV2);
        SPI.begin(CLK, -1, DIN, CS);

        // initialization sometimes fails with many chained displays
        // that's why we brute force it here
        for(u8 i = 0; i < 20; i++) initialize();
        setBrightness(brightness);
    }

    void clear() {
        for(size_t i = 0; i < 8; i++) transferAll(i + 1, 0);
    }

    void init() override {
        initialize();
    }

    void enable(bool enable) override {
        transferAll(0x0c, enable);
    }

    void setBrightness(u8 brightness) override {
        this->brightness = brightness & 0xf;
        transferAll(0x0a, this->brightness);
    }

    void drawColumn(u8 col, u8 value) override {
        u8 n = col >> 3;
        u8 c = (col & 7) + 1;
        digitalWrite(CS, LOW);
        for(size_t i = 0; i < Config::Displays::NUM_DISPLAYS; i++) {
            if(i == n) transfer(c, value); 
            else noop();
        }
        digitalWrite(CS, HIGH);
    }

    void drawBitmap(const u8* bitmap, size_t size, u8 position) override {
        const static i32 NUM_COLUMNS = Config::Displays::NUM_DISPLAYS * 8;
        for(i32 i = 0; i < size; i++) {
            i32 col = position + i;
            if(col >= 0 && col < NUM_COLUMNS) drawColumn(col, bitmap[i]);
        }
    }
};