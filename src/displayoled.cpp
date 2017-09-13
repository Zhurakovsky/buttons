#include "displayoled.hpp"

namespace rpibuttons
{

DisplayOled::DisplayOled()
    : m_textSize(2)
{
    m_opts.oled = OLED_ADAFRUIT_I2C_128x64;
    m_opts.verbose = 0;
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
      display.setTextSize(m_textSize);
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
    //display.clearDisplay();
    //display.display();
    display.setTextSize(m_textSize);
    display.setCursor(0,0);
    display.setTextWrap(false);
    m_currentVectorSize = menuItems.size();
    std::vector<MenuItem *> workPrintItems;

    if (m_currentActivePosition <= m_linesOnScreen)
    {
        m_currentMenuShift = 0;
        for (auto it = menuItems.begin(); it != (menuItems.begin() + m_linesOnScreen); ++it)
         {
             workPrintItems.push_back(static_cast<MenuItem *>(*it));
         }
    }
    else if (m_currentActivePosition > m_linesOnScreen)
    {
        m_currentMenuShift = m_currentActivePosition - m_linesOnScreen;
        for (auto it = (menuItems.begin() + m_currentMenuShift); it != (menuItems.begin() + m_currentMenuShift + m_linesOnScreen); ++it)
        {
            workPrintItems.push_back(static_cast<MenuItem *>(*it));
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
    if (m_currentActivePosition > 1)
    {
        m_currentActivePosition -= 1;
    }
}

void DisplayOled::resetCurrentActivePosition()
{
    m_currentActivePosition = 1;
    m_currentMenuShift = 0;
}


}


