#include "displayoled.hpp"

namespace rpibuttons
{

DisplayOled::DisplayOled()
    : m_textSize(2)
{
    std::cout << "[LOGS:DisplayOled] Constructor" << std::endl;
    m_opts.oled = OLED_ADAFRUIT_I2C_128x64;
    m_opts.verbose = 0;
    m_currentActivePosition = 0;
    m_currentMenuShift = 0;
    m_currentVectorSize = 0;
    m_linesOnScreen = 4;
}

DisplayOled::~DisplayOled()
{
    std::cout << "[LOGS:DisplayOled] Destructor" << std::endl;
    display.clearDisplay();   // clears the screen  buffer
    display.display();        // display it (clear display)
    display.close();
}

void DisplayOled::init()
{
    std::cout << "[LOGS:DisplayOled] Init()" << std::endl;
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
    clear();
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

void DisplayOled::drawBitmap(int16_t x, int16_t y, const std::vector<uint8_t>&bitmap, int16_t w, int16_t h, uint16_t color)
{
    //Store parameters of bitmap to local parameters
    //std::vector<uint8_t>bitmap;
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
    m_color = color;

    m_pictureBitmap.erase(m_pictureBitmap.begin(), m_pictureBitmap.end());
    m_pictureBitmap = bitmap;

    uint8_t* bitmapData = new uint8_t[m_w * m_h + 1];
    bitmapData = m_pictureBitmap.data();

    clear();
    display.drawBitmap(m_x, m_y, bitmapData, m_w, m_h, 1);
    display.display();
}

void DisplayOled::drawBitmap(int16_t shift_x, int16_t shift_y)
{
    if(!((m_x + shift_x) <= 0 || (m_x + shift_x) > 128))
    {
        m_x += shift_x;
    }
    if(!((m_y + shift_y) <= 0 || (m_y + shift_y) > 64))
    {
        m_y += shift_y;
    }

    uint8_t* bitmapData = new uint8_t[m_w * m_h + 1];
    bitmapData = m_pictureBitmap.data();

    clear();
    display.drawBitmap(m_x, m_y, bitmapData, m_w, m_h, m_color);
    display.display();
}

void DisplayOled::clear()
{
    display.fillScreen(0);
    display.display();
}

void DisplayOled::printText(const std::string &textToPrint)
{
    clear();
    display.setTextSize(m_textSize);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.setTextWrap(true);
    display.print(textToPrint.c_str());
    display.display();
}

}
