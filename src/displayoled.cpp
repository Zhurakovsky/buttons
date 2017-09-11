#include "displayoled.hpp"

namespace rpibuttons
{

DisplayOled::DisplayOled()
{
    m_opts.oled = OLED_ADAFRUIT_I2C_128x64;
    m_opts.verbose = 0;
}

DisplayOled::~DisplayOled()
{

}

void DisplayOled::init()
{
    // I2C change parameters to fit to your LCD
    if ( !display.init(OLED_I2C_RESET, m_opts.oled) )
    {
        std::cout << "Error Init Display" << std::endl;
        exit(EXIT_FAILURE); // TODO: do we need exit? I guess - yes
        //return;
    }
    display.begin();

    // init done
    display.clearDisplay();   // clears the screen  buffer
    display.display();        // display it (clear display)
    std::cout << "Init Display Done" << std::endl;
}

void DisplayOled::initTest()
{
    // text display tests
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("MenuItem1\n");
      display.display();
      //sleep(2);
      //display.clearDisplay();   // clears the screen  buffer
      //display.display();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      //display.setTextSize(1);
      display.print("MenuItem2\n");
      display.setTextColor(WHITE, BLACK); // 'non-inverted' text
      display.print("MenuItem3\n");
      //display.printf("%f\n", 3.141592);
      //display.setTextSize(2);
      //display.setTextColor(WHITE);
      //display.printf("0x%8X\n", 0xDEADBEEF);
      display.display();
      sleep(2);
}

void DisplayOled::close()
{
    display.close();
}

void DisplayOled::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    Adafruit_GFX::drawPixel(x, y, color);
}

void DisplayOled::invertDisplay(boolean i)
{
    Adafruit_GFX::invertDisplay(i);
}

void DisplayOled::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    Adafruit_GFX::drawLine(x0, y0, x1, y1, color);
}

void DisplayOled::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    Adafruit_GFX::drawFastVLine(x, y, h, color);
}

void DisplayOled::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    Adafruit_GFX::drawFastHLine(x, y, w, color);
}

void DisplayOled::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    Adafruit_GFX::drawRect(x, y, w, h, color);
}

void DisplayOled::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    Adafruit_GFX::fillRect(x, y, w, h, color);
}

void DisplayOled::fillScreen(uint16_t color)
{
    Adafruit_GFX::fillScreen(color);
}

uint32_t DisplayOled::getTextSize()
{
    return static_cast<uint32_t>(Adafruit_GFX::textsize);
}


}
