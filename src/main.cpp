#include <iostream>
#include "buttonListener.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"
#include "menubuilder.hpp"

#include <vector>
#include <map>
#include <iterator>
#include <functional>
#include <iterator>
#include <string>

#include <bcm2835.h>

const std::string configFile = "config.txt";

using namespace std;
using namespace rpibuttons;

void buttonUp();
void buttonDown();
void buttonLeft();
void buttonRight();
void buttonEnter();
void buttonEsc();

int main()
{
    ButtonListener bl;
    std::vector<MenuItem*> menu;
    {
//    MenuItem *mi0 = new MenuItem();
//    mi0->setActive(true);
//    mi0->setItemName("Item 0");
//    menu.push_back(mi0);

//    MenuItem *mi1 = new MenuItem(1);
//    mi0->setNext(mi1);
//    //mi1->setActive(true);
//    mi1->setItemName("Item 1");
//    mi1->setPrevious(mi0);
//    menu.push_back(mi1);

//    MenuItem *mi2 = new MenuItem(2);
//    mi1->setNext(mi2);
//    //mi1->setActive(true);
//    mi2->setItemName("Item 2");
//    mi2->setPrevious(mi1);
//    menu.push_back(mi2);
}

    //Build menu
    MenuBuilder mBuilder;
    mBuilder.buildMenu(configFile, menu);

    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *tmpItem = *it;
        std::cout << tmpItem->itemName().c_str() << std::endl;
    }

    // Assign input GPIO PINS according to config
    // First argument - phisically pin number on RPi
    // Secong - bcm2835 shift value
    std::map<int, int> mapPinGpio;
    mBuilder.buildPinGpioMap(configFile, mapPinGpio);

    std::function<void()> callbackButtonUp = buttonUp;
    std::function<void()> callbackButtonDown = buttonDown;
    std::function<void()> callbackButtonLeft = buttonLeft;
    std::function<void()> callbackButtonRight = buttonRight;
    std::function<void()> callbackButtonEnter = buttonEnter;
    std::function<void()> callbackButtonEsc = buttonEsc;

    std::map<int, std::string> mapButtonsFuncAssigned;
    mBuilder.buildButtonsFuncAssigned(configFile, mapButtonsFuncAssigned);

    for (auto it = mapButtonsFuncAssigned.begin(); it != mapButtonsFuncAssigned.end(); ++it)
    {
        int pinAssigned = it->first;
        std::string funcAssigned = it->second;

        std::cout << pinAssigned << " pin assigned to func " << funcAssigned.c_str() << std::endl;

        auto iter = mapPinGpio.find(pinAssigned);
        if (iter != mapPinGpio.end())
        {
            int shiftAssigned = iter->second;
            std::cout << " iter->second " << shiftAssigned << std::endl;
            if (funcAssigned.compare("UP") == 0)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonUp);
            }
            else if (funcAssigned.compare("DOWN") == 0)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonDown);
            }
            else if (funcAssigned.compare("LEFT") == 0)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonLeft);
            }
            else if (funcAssigned.compare("RIGHT") == 0)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonRight);
            }
            else if (funcAssigned.compare("ENTER") == 0)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonEnter);
            }
            else if (funcAssigned.compare("ESC") == 0)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonEsc);
            }
            else
            {
                std::cout << " No FUNC found " << std::endl;
            }
        }
    }

    bl.run();
    std::cout << "Hello, Buttons" << std::endl;
    return 0;
}

void buttonUp()
{
    std::cout << "Button UP pressed" << std::endl;
}
void buttonDown()
{
    std::cout << "Button DOWN pressed" << std::endl;
}
void buttonLeft()
{
    std::cout << "Button LEFT pressed" << std::endl;
}
void buttonRight()
{
    std::cout << "Button RIGHT pressed" << std::endl;
}
void buttonEnter()
{
    std::cout << "Button ENTER pressed" << std::endl;
}
void buttonEsc()
{
    std::cout << "Button ESC pressed" << std::endl;
}
