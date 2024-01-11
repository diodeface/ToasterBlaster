#pragma once

#include "Utilities/Typedefs.h"
#include "Utilities/Util.h"
#include "Components/ToasterComponent.h"

// structs for holding information about hud elements
namespace HUDElement {
    struct Text {
        u8 textX, textY, padding;
        const u8* font;
    };

    struct Bar {
        u8 x, y, w, h;
    };

    struct Element {
        u8 x, y;
    };
}

class HeadsUpDisplay : public ToasterComponent {
  public:
    virtual void clearRect(u8 x, u8 y, u8 w, u8 h) = 0;

    virtual u8 getDrawColor() = 0;

    virtual void clearBottom() = 0;

    virtual void drawIcon(u8 x, u8 y, Bitmap* icon) = 0;

    virtual void drawBitmap(const u8* bitmap, size_t size, u8 x, u8 y) = 0;

    virtual void drawBitmap(const u8* bitmap, size_t size, Point position) = 0;

    virtual void setFont(const u8* font) = 0;

    virtual void drawStr(u8 x, u8 y, const char* s) = 0;

    virtual void drawStrX2(u8 x, u8 y, const char* s) = 0;

    virtual void drawFrame(u8 x, u8 y, u8 w, u8 h) = 0;

    virtual void drawBox(u8 x, u8 y, u8 w, u8 h) = 0;

    virtual void setDrawColor(u8 c) = 0;

    virtual void drawHLine(u8 x, u8 y, u8 w) = 0;

    virtual void drawVLine(u8 x, u8 y, u8 h) = 0;

    virtual void drawStatusText(const char* str) = 0;

    virtual void update() {}

    virtual void drawIcon(const HUDElement::Element& el, const u8* icon) {
        drawIcon(el.x, el.y, icon);
    }

    void drawBar(const HUDElement::Bar& bar, u8 width) {
        if(width > bar.w) width = bar.w;

        setDrawColor(0);
        drawBox(bar.x, bar.y, bar.w, bar.h);
        setDrawColor(1);
        drawBox(bar.x, bar.y, width, bar.h);
    }

    void drawText(const HUDElement::Text& text, const char* str) {
        char buf[64];
        snprintf(buf, 64, "%*s", text.padding, str);
        setFont(text.font);
        drawStr(text.textX, text.textY, buf);
    }

    void drawText(const HUDElement::Text& text, u8 val) {
        char buf[4];
        snprintf(buf, 4, "%d", val);
        drawText(text, buf);
    }

    void drawText(u8 x, u8 y, u8 val) {
        char buf[4];
        snprintf(buf, 4, "%d", val);
        drawStr(x, y, buf);
    }

    void drawCenteredText(u8 x, u8 y, u8 maxChars, const char* str) {
        char buf[maxChars];
        u8 len = strlen(str);
        u8 padding = (maxChars - len) / 2 + len;
        snprintf(buf, maxChars, "%*s", padding, str);
        drawStr(x, y, buf);
    }

    void drawCross(u8 x, u8 y, u8 w, u8 h, u8 lineLength) {
        // top left
        drawHLine(x, y, lineLength);
        drawVLine(x, y, lineLength);

        // top right
        drawHLine(x + w - lineLength, y, lineLength);
        drawVLine(x + w - 1, y, lineLength);

        // bottom left
        drawHLine(x, y + h - 1, lineLength);
        drawVLine(x, y + h - lineLength, lineLength);

        // bottom right
        drawHLine(x + w - lineLength, y + h - 1, lineLength);
        drawVLine(x + w - 1, y + h - lineLength, lineLength);
    }
};