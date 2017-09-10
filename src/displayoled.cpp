#include "displayoled.hpp"

namespace rpibuttons
{

DisplayOled::DisplayOled()
{
    m_opts.oled = OLED_ADAFRUIT_SPI_128x64;
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
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("Hello, world!\n");
      display.display();
      sleep(2);
      display.clearDisplay();   // clears the screen  buffer
      display.display();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.setTextSize(2);
      display.print("Hello, world!\n");
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


}
