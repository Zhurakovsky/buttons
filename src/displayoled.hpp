#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include "commontypes.hpp"
#include "menuitem.hpp"

#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"


namespace rpibuttons
{

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
/*
static const unsigned char logo_bmp[] =
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
//static const unsigned char logo_bmp[] =
//{
//    0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00};

class DisplayOled
{
public:
    DisplayOled(const DisplayOled&) = delete;
    DisplayOled& operator=(const DisplayOled&) = delete;

    DisplayOled();
    ~DisplayOled();

    void init();
    void initTest();
    void close();
    uint32_t getTextSize();
    void setTextSize(uint32_t newSize);
    void printMenuList(const std::vector<MenuItem *> &menuItems);


    uint32_t getLinesOnScreen();
    uint32_t getCurrentActivePosition();
    uint32_t getCurrentMenuShift();

    void increaseCurrentActivePosition(const std::vector<MenuItem *> &menuItems);
    void decreaseCurrentActivePosition(const std::vector<MenuItem *> &menuItems);
    void resetCurrentActivePosition();

    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
    void clear();
    void printText(const std::string &textToPrint);

private:

    // default options values
    s_opts m_opts;
    // Instantiate the display
    ::ArduiPi_OLED display;
    uint32_t m_textSize;
    uint32_t m_linesOnScreen;
    uint32_t m_currentActivePosition;
    uint32_t m_currentMenuShift;
    uint32_t m_currentVectorSize;
};
}
