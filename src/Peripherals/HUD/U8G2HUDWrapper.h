#pragma once

#include "HeadsUpDisplay.h"
#include <U8g2lib.h>

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
    TaskHandle_t hudTask;
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2;

    void convertXBM(const u8* source, u8* target, size_t size) {
        i32 w = size / 8;
        for(i32 i = 0; i < 8; i++) {
            for(i32 j = 0; j < w; j++) {
                target[i * w + j] = mirrorInt(source[i + 8 * j]);
            }
        }
    }

  public:
    U8G2HUDWrapper(u8 scl, u8 sda) {
        u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, scl, sda);
        u8g2->begin();
        u8g2->firstPage();
        u8g2->clear();
        u8g2->setFontMode(0);
        u8g2->setFont(u8g2_font_5x7_mr);     // todo: hardcoded font?
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
        u8g2->drawStr(x, y, s);
    }

    void drawStrX2(u8 x, u8 y, const char* s) override {
        u8g2->drawStrX2(x, y, s);
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

    void update() override {
        u8g2->nextPage();
    }
};