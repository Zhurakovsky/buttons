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

int main()
{
    ButtonListener bl;
    std::vector<MenuItem*> menu;

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

    std::function<void()> callbackButtonUp = [&]()
    {
        std::cout << "Button UP pressed" << std::endl;
    };
    std::function<void()> callbackButtonDown = [&]()
    {
        std::cout << "Button DOWN pressed" << std::endl;
    };
    std::function<void()> callbackButtonLeft = [&]()
    {
        std::cout << "Button LEFT pressed" << std::endl;
    };
    std::function<void()> callbackButtonRight = [&]()
    {
        std::cout << "Button RIGHT pressed" << std::endl;
    };
    std::function<void()> callbackButtonEnter = [&]()
    {
        std::cout << "Button ENTER pressed" << std::endl;
    };
    std::function<void()> callbackButtonEsc = [&]()
    {
        std::cout << "Button ESC pressed" << std::endl;
    };

    std::map<int, std::string> mapButtonsFuncAssigned;
    mBuilder.buildButtonsFuncAssigned(configFile, mapButtonsFuncAssigned);

    for (auto it = mapButtonsFuncAssigned.begin(); it != mapButtonsFuncAssigned.end(); ++it)
    {
        int pinAssigned = it->first;
        const std::string funcAssigned(it->second);

        std::cout << int(it->first) << " pin assigned to func " << funcAssigned.c_str() << std::endl;

        auto iter = mapPinGpio.find(pinAssigned);
        if (iter != mapPinGpio.end())
        {
            int shiftAssigned = iter->second;
            if (funcAssigned == UP)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonUp);
            }
            else if (funcAssigned == DOWN)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonDown);
            }
            else if (funcAssigned == LEFT)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonLeft);
            }
            else if (funcAssigned == RIGHT)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonRight);
            }
            else if (funcAssigned == ENTER)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonEnter);
            }
            else if (funcAssigned == ESC)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonEsc);
            }
            else
            {
                std::cout << " No FUNC found for " << funcAssigned.c_str() << std::endl;
            }
        }
    }

    bl.run();
    while(1)
    {}
    return 0;
}
