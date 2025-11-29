#pragma once

#include <U8g2lib.h>
#include "HeadsUpDisplay.h"

// structs for holding information about hud elements
namespace HUDElement {
    const Element iconMicrophone{112, 8};
    const Element iconBoop{66, 8};

    const Element statusMicrophone{120, 8};
    const Element statusBoop{74, 8};

    const Text textController{68, 7, 12, u8g2_font_5x7_mr};
    const Text textUptime{103, 15, 0, u8g2_font_5x7_mr};
    const Text textBoop{68, 15, 12, u8g2_font_5x7_mr};

    const Bar barMicrophone{114, 8, 16, 8};
    const Bar barBoop{74, 0, 8, 8};
}

/**
 * U8G2 wrapper for heads up display (work in progress)
 * I recommend calling updates on a separate core to avoid slowdown
 */
class U8G2HUDWrapper : public HeadsUpDisplay {
  private:
    u8 *mainBuffer, *subBuffer;

    // i know i know
    // we are using two instances of u8g2, one for drawing to the buffers, one for actually rendering the result to the display
    // u8g2's direct buffer access API cannot work with a single instance since rendering is done asynchcronously on a separate core
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2;
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2Renderer;

    void convertXBM(const u8* source, u8* target, size_t size) {
        i32 w = size / 8;
        for(i32 i = 0; i < 8; i++) {
            for(i32 j = 0; j < w; j++) {
                target[i * w + j] = mirrorInt(source[i + 8 * j]);
            }
        }
    }

    u8* getBufferPtr(HUDBuffer buffer) {
        switch (buffer) {
            case HUD_BUFFER_SUB:    return subBuffer;
            default:                return mainBuffer;
        }
    }

  public:
    U8G2HUDWrapper(u8 scl, u8 sda) {
        u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);
        u8g2Renderer = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, scl, sda);
        
        mainBuffer = (u8*)malloc(u8g2->getBufferSize());
        memset(mainBuffer, 0, u8g2->getBufferSize());
        subBuffer = (u8*)malloc(u8g2->getBufferSize());
        memset(subBuffer, 0, u8g2->getBufferSize());

        u8g2->setBufferPtr(mainBuffer);
        u8g2Renderer->setBufferPtr(mainBuffer);
        u8g2Renderer->initDisplay();
        u8g2Renderer->clearDisplay();
        u8g2Renderer->setPowerSave(0);
        u8g2Renderer->setFontMode(0);
    }

    void clearRect(u8 x, u8 y, u8 w, u8 h) override {
        u8 prevDrawColor = getDrawColor();
        setDrawColor(0);
        drawBox(x, y, w, h);
        setDrawColor(prevDrawColor);
    }

    u8 getDrawColor() override {
        return u8g2->getDrawColor();
    }

    void clearBottom() override {
        clearRect(0, 16, 128, 48);
    }

    void drawIcon(u8 x, u8 y, Bitmap* icon) override {
        u8g2->drawXBM(x, y, 8, 8, icon);
    }

    void drawBitmap(const u8* bitmap, size_t size, u8 x, u8 y) override {
        u8 xbm[size];
        convertXBM(bitmap, xbm, size);
        u8g2->drawXBM(x, y, size, 8, xbm);
    }

    void drawXBM(u8 x, u8 y, u8 w, u8 h, Bitmap* xbm) {
        u8g2->drawXBM(x, y, w, h, xbm);
    }

    void drawBitmap(const u8* bitmap, size_t size, Point position) override {
        drawBitmap(bitmap, size, position.x, position.y);
    }

    void drawStatusText(const char* str) override {
        drawText(HUDElement::textController, str);
    }

    // wrappers
    void setFont(const u8* font) override {
        u8g2->setFont(font);
    }

    void drawStr(u8 x, u8 y, const char* s) override {
        u8g2->setFont(u8g2_font_5x7_mr);     // todo: hardcoded font?
        u8g2->drawStr(x, y, s);
    }

    void drawStrX2(u8 x, u8 y, const char* s) override {
        u8g2->setFont(u8g2_font_9x15_mr);     // todo: hardcoded font?
        u8g2->drawStr(x, y, s);
    }

    void drawStrX2(u8 x, u8 y, bool bold, const char* s) override {
        if(!bold) drawStrX2(x, y, s);
        u8g2->setFont(u8g2_font_9x15B_mr);    // todo: hardcoded font?
        u8g2->drawStr(x, y, s);
    }

    void drawFrame(u8 x, u8 y, u8 w, u8 h) override {
        u8g2->drawFrame(x, y, w, h);
    }

    void drawBox(u8 x, u8 y, u8 w, u8 h) override {
        u8g2->drawBox(x, y, w, h);
    }

    void setDrawColor(u8 c) override {
        u8g2->setDrawColor(c);
    }

    void drawHLine(u8 x, u8 y, u8 w) override {
        u8g2->drawHLine(x, y, w);
    }

    void drawVLine(u8 x, u8 y, u8 h) override {
        u8g2->drawVLine(x, y, h);
    }

    void drawLine(u8 x1, u8 y1, u8 x2, u8 y2) override {
        u8g2->drawLine(x1, y1, x2, y2);
    }

    void setDrawBuffer(HUDBuffer buffer) override {
        u8g2->setBufferPtr(getBufferPtr(buffer));
    }

    void setRenderedBuffer(HUDBuffer buffer) override {
        u8g2Renderer->setBufferPtr(getBufferPtr(buffer));
    }

    void update() override {
        u8g2Renderer->sendBuffer();
    }
};