#include "displayoled.hpp"

namespace rpibuttons
{

DisplayOled::DisplayOled()
    : m_textSize(2)
{
    m_opts.oled = OLED_ADAFRUIT_I2C_128x64;
    m_opts.verbose = 0;
    m_currentActivePosition = 0;
    m_currentMenuShift = 0;
    m_currentVectorSize = 0;
    m_linesOnScreen = 4;
}

DisplayOled::~DisplayOled()
{
    display.clearDisplay();   // clears the screen  buffer
    display.display();        // display it (clear display)
    display.close();
}

void DisplayOled::init()
{
    // I2C change parameters to fit to your LCD
    if ( !display.init(OLED_I2C_RESET, m_opts.oled) )
    {
        std::cout << "Error Init Display" << std::endl;
        exit(EXIT_FAILURE); // TODO: do we need exit? I guess - yes
    }
    display.begin();

    // init done
    display.clearDisplay();   // clears the screen  buffer
    display.display();        // display it (clear display)
}

void DisplayOled::initTest()
{
    // text display tests
      display.setTextSize(m_textSize);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("MenuItem1\n");
      display.display();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.print("MenuItem2\n");
      display.setTextColor(WHITE, BLACK); // 'non-inverted' text
      display.print("MenuItem3\n");
      display.display();
      sleep(2);
}

void DisplayOled::close()
{
    display.close();
}

uint32_t DisplayOled::getTextSize()
{
    return m_textSize;
}

void DisplayOled::setTextSize(uint32_t newSize)
{
    m_textSize = newSize;
    switch (m_textSize)
    {
    case 1:
        m_linesOnScreen = 8;
        break;
    case 2:
        m_linesOnScreen = 4;
        break;
    case 3:
        m_linesOnScreen = 2;
        break;
    case 4:
        m_linesOnScreen = 1;
        break;
    default:
        break;
    }
}

void DisplayOled::printMenuList(const std::vector<MenuItem *> &menuItems)
{
    display.clearDisplay();
    display.display();
    display.setTextSize(m_textSize);
    display.setCursor(0,0);
    display.setTextWrap(false);
    m_currentVectorSize = menuItems.size();
    std::vector<MenuItem *> workPrintItems;

    if (m_currentActivePosition <= m_linesOnScreen)
    {
        m_currentMenuShift = 0;
        int lines = (menuItems.size() > m_linesOnScreen) ? m_linesOnScreen : menuItems.size();
        for (auto it = menuItems.begin(); it != (menuItems.begin() + lines); ++it)
         {
             MenuItem * tmpItem = *it;
             workPrintItems.push_back(tmpItem);
         }

    }
    else if (m_currentActivePosition > m_linesOnScreen)
    {
        m_currentMenuShift = m_currentActivePosition - m_linesOnScreen;
        int lines = ((menuItems.size() - m_currentMenuShift)  > m_linesOnScreen) ? m_linesOnScreen : (menuItems.size() - m_currentMenuShift);
        for (auto it = (menuItems.begin() + m_currentMenuShift); it != (menuItems.begin() + m_currentMenuShift + lines); ++it)
        {
            MenuItem * tmpItem = *it;
            workPrintItems.push_back(tmpItem);
        }

    }

    for (auto it = workPrintItems.begin(); it != workPrintItems.end(); ++it)
    {
        MenuItem* tmpItem = *it;
        if (tmpItem->isActive())
        {
            display.setTextColor(BLACK, WHITE);
        }
        else
        {
            display.setTextColor(WHITE, BLACK);
        }
        std::string stringItemName = tmpItem->itemName();
        display.printf("%s\n", stringItemName.c_str());

    }
    display.display();
}

uint32_t DisplayOled::getLinesOnScreen()
{
    return m_linesOnScreen;
}

uint32_t DisplayOled::getCurrentActivePosition()
{
    return m_currentActivePosition;
}

uint32_t DisplayOled::getCurrentMenuShift()
{
    return m_currentMenuShift;
}

void DisplayOled::increaseCurrentActivePosition(const std::vector<MenuItem *> &menuItems)
{
    m_currentVectorSize = menuItems.size();
    if (m_currentActivePosition < m_currentVectorSize)
    {
        m_currentActivePosition += 1;
    }
}

void DisplayOled::decreaseCurrentActivePosition(const std::vector<MenuItem *> &menuItems)
{
    m_currentVectorSize = menuItems.size();
    if (m_currentActivePosition > 0)
    {
        m_currentActivePosition -= 1;
    }
}

void DisplayOled::resetCurrentActivePosition()
{
    m_currentActivePosition = 0;
    m_currentMenuShift = 0;
}

void DisplayOled::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{


    uint8_t icons[NUMFLAKES][3];
    srandom(666);     // whatever seed
    (void)bitmap;

    // initialize
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      icons[f][XPOS] = random() % display.width();
      icons[f][YPOS] = 0;
      icons[f][DELTAY] = random() % 5 + 1;

      printf("x: %d", icons[f][XPOS]);
      printf("y: %d", icons[f][YPOS]);
      printf("dy: %d\n", icons[f][DELTAY]);
    }

    while (1) {
      // draw each icon
      for (uint8_t f=0; f< NUMFLAKES; f++) {
        display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo_bmp, w, h, WHITE);
      }
      display.display();
      usleep(100000/sleep_divisor);

      // then erase it + move it
      for (uint8_t f=0; f< NUMFLAKES; f++) {
        display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo_bmp, w, h, BLACK);
        // move it
        icons[f][YPOS] += icons[f][DELTAY];
        // if its gone, reinit
        if (icons[f][YPOS] > display.height()) {
      icons[f][XPOS] = random() % display.width();
      icons[f][YPOS] = 0;
      icons[f][DELTAY] = random() % 5 + 1;
        }
      }
     }
}

void DisplayOled::clear()
{
    display.fillScreen(0);
}

void DisplayOled::printText(const std::string &textToPrint)
{
    display.setTextWrap(true);
    display.print(textToPrint.c_str());
}

}
