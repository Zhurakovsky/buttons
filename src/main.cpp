#include <iostream>
#include "buttonListener.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"
#include "menubuilder.hpp"
#include "menuprocessor.hpp"

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

//void buttonUp();
//void buttonDown();
//void buttonLeft();
//void buttonRight();
//void buttonEnter();
//void buttonEsc();

int main()
{
    ButtonListener bl;
    std::vector<MenuItem*> menu;

    //Build menu
    MenuBuilder mBuilder;
    mBuilder.buildMenu(configFile, menu);

    MenuProcessor menuProcessor(menu);

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

    //std::cout << "Size of mapPinGpio map == " << mapPinGpio.size() << std::endl;

//    for (auto it = mapPinGpio.begin(); it != mapPinGpio.end(); ++it)
//    {
//        std::cout << it->first << " " << it->second << std::endl;
//    }

    std::function<void()> callbackButtonUp = [&]()
    {
        std::cout << "Button UP pressed" << std::endl;
        if (!menuProcessor.menuHasActive())
        {
            std::cout << "No ective item" << std::endl;
            return;
        }
        MenuItem *item = menuProcessor.getActive();
        std::cout << "Live 1" << std::endl;
        if (item->hasPrevious())
        {
            std::cout << "Live 2" << std::endl;
            item->setActive(false);
            item = item->getPrevious();
            item->setActive(true);
        }
        else
        {
            std::cout << "Live 3" << std::endl;
            std::cout << "Top UP item" << std::endl;
        }
        std::cout << "Live 4" << std::endl;
        menuProcessor.printMenu();
    };
    std::function<void()> callbackButtonDown = [&]()
    {
        std::cout << "Button DOWN pressed" << std::endl;
        if (!menuProcessor.menuHasActive())
        {
            std::cout << "No ective item" << std::endl;
            return;
        }
        MenuItem *item = menuProcessor.getActive();
        if (item->getNext())
        {
            item->setActive(false);
            item = item->getNext();
            item->setActive(true);
        }
        else
        {
            std::cout << "Last BOTTOM item" << std::endl;
        }
        menuProcessor.printMenu();
    };
    std::function<void()> callbackButtonLeft = [&]()
    {
        std::cout << "Button LEFT pressed" << std::endl;
        if (!menuProcessor.menuHasActive())
        {
            std::cout << "No ective item" << std::endl;
            return;
        }
        MenuItem *item = menuProcessor.getActive();
        if (item->hasParent())
        {
            item->setActive(false);
            item = item->getParent();
            item->setActive(true);
        }
        else
        {
            std::cout << "No parent item" << std::endl;
        }
        menuProcessor.printMenu();
    };
    std::function<void()> callbackButtonRight = [&]()
    {
        std::cout << "Button RIGHT pressed" << std::endl;
        if (!menuProcessor.menuHasActive())
        {
            std::cout << "No ective item" << std::endl;
            return;
        }
        MenuItem *item = menuProcessor.getActive();
        if (item->hasChild())
        {
            item->setActive(false);
            item = item->getChild();
            item->setActive(true);
        }
        else
        {
            std::cout << "No child item" << std::endl;
        }
        menuProcessor.printMenu();
    };
    std::function<void()> callbackButtonEnter = [&]()
    {
        std::cout << "Button ENTER pressed" << std::endl;
        std::cout << "Realisation under construct" << std::endl;
        menuProcessor.printMenu();
    };
    std::function<void()> callbackButtonEsc = [&]()
    {
        std::cout << "Button ESC pressed" << std::endl;
        std::cout << "Realisation under construct" << std::endl;
        menuProcessor.printMenu();
    };

    std::map<int, std::string> mapButtonsFuncAssigned;
    mBuilder.buildButtonsFuncAssigned(configFile, mapButtonsFuncAssigned);

    //std::cout << "Size of mapButtonsFuncAssigned map == " << mapButtonsFuncAssigned.size() << std::endl;

//    for (auto it = mapButtonsFuncAssigned.begin(); it != mapButtonsFuncAssigned.end(); ++it)
//    {
//        int intValue = it->first;
//        std::string tmpString = it->second;
//        std::cout << intValue << " " << tmpString.c_str() << std::endl;
//    }

    for (auto it = mapButtonsFuncAssigned.begin(); it != mapButtonsFuncAssigned.end(); ++it)
    {
        int pinAssigned = it->first;
        const std::string funcAssigned(it->second);

        std::cout << int(it->first) << " pin assigned to func " << funcAssigned.c_str() << std::endl;

        auto iter = mapPinGpio.find(pinAssigned);
        if (iter != mapPinGpio.end())
        {
            int shiftAssigned = iter->second;
            //std::cout << " iter->second " << shiftAssigned << std::endl;
            if (funcAssigned == UP)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonUp);
                //std::cout << " found assign!!! " << UP << std::endl;
            }
            else if (funcAssigned == DOWN)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonDown);
                //std::cout << " found assign!!! " << DOWN << std::endl;
            }
            else if (funcAssigned == LEFT)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonLeft);
                //std::cout << " found assign!!! " << LEFT << std::endl;
            }
            else if (funcAssigned == RIGHT)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonRight);
                //std::cout << " found assign!!! " << RIGHT << std::endl;
            }
            else if (funcAssigned == ENTER)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonEnter);
                //std::cout << " found assign!!! " << ENTER << std::endl;
            }
            else if (funcAssigned == ESC)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonEsc);
                //std::cout << " found assign!!! " << ESC << std::endl;
            }
            else
            {
                std::cout << " No FUNC found for " << funcAssigned.c_str() << std::endl;
            }
        }
    }
    menuProcessor.initActiveMenuItem();

    bl.run();
    while(1)
    {}
    return 0;
}

//void buttonUp()
//{
//    std::cout << "Button UP pressed" << std::endl;
//}
//void buttonDown()
//{
//    std::cout << "Button DOWN pressed" << std::endl;
//}
//void buttonLeft()
//{
//    std::cout << "Button LEFT pressed" << std::endl;
//}
//void buttonRight()
//{
//    std::cout << "Button RIGHT pressed" << std::endl;
//}
//void buttonEnter()
//{
//    std::cout << "Button ENTER pressed" << std::endl;
//}
//void buttonEsc()
//{
//    std::cout << "Button ESC pressed" << std::endl;
//}
