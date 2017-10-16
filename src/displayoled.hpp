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

#define OLED_ADAFRUIT_I2C_128x64  3


class DisplayOled
{
public:
    //DisplayOled(const DisplayOled&) = delete;
    //DisplayOled& operator=(const DisplayOled&) = delete;

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

    void drawBitmap(int16_t x, int16_t y, const std::vector<uint8_t>&bitmap, int16_t w, int16_t h, uint16_t color);
    void drawBitmap(int16_t shift_x, int16_t shift_y);
    void clear();
    void printText(const std::string &textToPrint);

private:
    //std::mutex m_mtx;
    // default options values
    s_opts m_opts;
    // Instantiate the display
    ArduiPi_OLED display;
    uint32_t m_textSize;
    uint32_t m_linesOnScreen;
    uint32_t m_currentActivePosition;
    uint32_t m_currentMenuShift;
    uint32_t m_currentVectorSize;

    int16_t m_x;
    int16_t m_y;
    std::vector<uint8_t>m_pictureBitmap;
    //std::vector<uint8_t>bitmap;
    int16_t m_w;
    int16_t m_h;
    uint16_t m_color;
};
}
