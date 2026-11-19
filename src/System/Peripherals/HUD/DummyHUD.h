#pragma once

#include "HeadsUpDisplay.h"

/**
 * Dummy HUD for when no heads up display is used
 */
class DummyHUD : public HeadsUpDisplay {
  public:
    DummyHUD() {
        return;
    }

    void clearRect(u8 x, u8 y, u8 w, u8 h) override {
        return;
    }

    u8 getDrawColor() override {
        return 0;
    }

    void clearBottom() override {
        return;
    }

    void drawIcon(u8 x, u8 y, Bitmap* icon) override {
        return;
    }

    void drawBitmap(const u8* bitmap, size_t size, u8 x, u8 y) override {
        return;
    }

    void drawXBM(u8 x, u8 y, u8 w, u8 h, Bitmap* xbm) override {
        return;
    }

    void drawBitmap(const u8* bitmap, size_t size, Point position) override {
        return;
    }

    void drawStatusText(const char* str) override {
        return;
    }

    void setFont(const u8* font) override {
        return;
    }

    void drawStr(u8 x, u8 y, const char* s) override {
        return;
    }

    void drawStrX2(u8 x, u8 y, const char* s) override {
        return;
    }

    void drawStrX2(u8 x, u8 y, bool bold, const char* s) override {
        return;
    }

    void drawFrame(u8 x, u8 y, u8 w, u8 h) override {
        return;
    }

    void drawBox(u8 x, u8 y, u8 w, u8 h) override {
        return;
    }

    void setDrawColor(u8 c) override {
        return;
    }

    void drawHLine(u8 x, u8 y, u8 w) override {
        return;
    }

    void drawVLine(u8 x, u8 y, u8 h) override {
        return;
    }

    void drawLine(u8 x1, u8 y1, u8 x2, u8 y2) override {
        return;
    }

    void setDrawBuffer(HUDBuffer buffer) override {
        return;
    }

    void setRenderedBuffer(HUDBuffer buffer) override {
        return;
    }

    void update() override {
        return;
    }
};