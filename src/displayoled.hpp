#pragma once

#include <iostream>

#include "commontypes.hpp"

#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
/*
static unsigned char logo16_glcd_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };
*/

namespace rpibuttons
{

class DisplayOled : public Adafruit_GFX
{
public:
    DisplayOled(const DisplayOled&) = delete;
    DisplayOled& operator=(const DisplayOled&) = delete;

    DisplayOled();
    ~DisplayOled();

    void init();
    void initTest();
    void close();

    // this must be defined by the subclass
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void invertDisplay(boolean i);

    // these are 'generic' drawing functions, so we can share them!
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    virtual void fillScreen(uint16_t color);

    uint32_t getTextSize();
private:


    // default options values
    s_opts m_opts;
    // Instantiate the display
    ::ArduiPi_OLED display;

};
}
