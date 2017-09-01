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

<<<<<<< HEAD
//void buttonUp(MenuProcessor &menuProcessor);
//void buttonDown();
//void buttonLeft();
//void buttonRight();
//void buttonEnter();
//void buttonEsc();
=======
void buttonUp();
void buttonDown();
void buttonLeft();
void buttonRight();
void buttonEnter();
void buttonEsc();
>>>>>>> parent of 076a533... New class menuprocessor added to project

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

    std::cout << "Size of mapPinGpio map == " << mapPinGpio.size() << std::endl;

<<<<<<< HEAD
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
//        MenuItem *item = menuProcessor.getActive();
//        if (!item)
//        {
//            std::cout << "item is NULL" << std::endl;
//            return;
//        }
//        if (item->getPrevious())
//        {
//            item->setActive(false);
//            item = item->getPrevious();
//            item->setActive(true);
//        }
//        else
//        {
//            std::cout << "Top UP item" << std::endl;
//        }
        menuProcessor.printMenu();
    };
    std::function<void()> callbackButtonDown = [&]()
    {
        std::cout << "Button DOWN pressed" << std::endl;
        if (!menuProcessor.menuHasActive())
        {
            std::cout << "No active item" << std::endl;
            return;
        }
//        MenuItem *item = menuProcessor.getActive();
//        if (!item)
//        {
//            std::cout << "active item is NULL" << std::endl;
//            return;
//        }
//        if (item->getNext())
//        {
//            item->setActive(false);
//            item = item->getNext();
//            item->setActive(true);
//        }
//        else
//        {
//            std::cout << "Last BOTTOM item" << std::endl;
//        }
        menuProcessor.printMenu();
    };
    std::function<void()> callbackButtonLeft = [&]()
    {
        std::cout << "Button LEFT pressed" << std::endl;
        if (!menuProcessor.menuHasActive())
        {
            std::cout << "No active item" << std::endl;
            return;
        }
//        MenuItem *item = menuProcessor.getActive();
//        if (!item)
//        {
//            std::cout << "active item is NULL" << std::endl;
//            return;
//        }
//        if ((item->getParent() != NULL))
//        {
//            item->setActive(false);
//            item = item->getParent();
//            item->setActive(true);
//        }
//        else
//        {
//            std::cout << "No parent item" << std::endl;
//        }
        menuProcessor.printMenu();
    };
    std::function<void()> callbackButtonRight = [&]()
    {
        std::cout << "Button RIGHT pressed" << std::endl;
        if (!menuProcessor.menuHasActive())
        {
            std::cout << "No active item" << std::endl;
            return;
        }
//        MenuItem *item = menuProcessor.getActive();
//        if (!item)
//        {
//            std::cout << "active item is NULL" << std::endl;
//            return;
//        }
//        if ((item->getChild() != NULL))
//        {
//            item->setActive(false);
//            item = item->getChild();
//            item->setActive(true);
//        }
//        else
//        {
//            std::cout << "No child item" << std::endl;
//        }
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
=======
    for (auto it = mapPinGpio.begin(); it != mapPinGpio.end(); ++it)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }

    std::function<void()> callbackButtonUp = buttonUp;
    std::function<void()> callbackButtonDown = buttonDown;
    std::function<void()> callbackButtonLeft = buttonLeft;
    std::function<void()> callbackButtonRight = buttonRight;
    std::function<void()> callbackButtonEnter = buttonEnter;
    std::function<void()> callbackButtonEsc = buttonEsc;
>>>>>>> parent of 076a533... New class menuprocessor added to project

    std::map<int, std::string> mapButtonsFuncAssigned;
    mBuilder.buildButtonsFuncAssigned(configFile, mapButtonsFuncAssigned);

    std::cout << "Size of mapButtonsFuncAssigned map == " << mapButtonsFuncAssigned.size() << std::endl;

    for (auto it = mapButtonsFuncAssigned.begin(); it != mapButtonsFuncAssigned.end(); ++it)
    {
        int intValue = it->first;
        std::string tmpString = it->second;
        std::cout << intValue << " " << tmpString.c_str() << std::endl;
    }

    for (auto it = mapButtonsFuncAssigned.begin(); it != mapButtonsFuncAssigned.end(); ++it)
    {
        int pinAssigned = it->first;
        const std::string funcAssigned(it->second);


        std::cout << pinAssigned << " pin assigned to func " << funcAssigned.c_str() << std::endl;

        auto iter = mapPinGpio.find(pinAssigned);
        if (iter != mapPinGpio.end())
        {
            int shiftAssigned = iter->second;
            std::cout << " iter->second " << shiftAssigned << std::endl;
            if (funcAssigned == UP)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonUp);
                std::cout << " found assign!!! " << UP << std::endl;
            }
            else if (funcAssigned == DOWN)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonDown);
                std::cout << " found assign!!! " << DOWN << std::endl;
            }
            else if (funcAssigned == LEFT)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonLeft);
                std::cout << " found assign!!! " << LEFT << std::endl;
            }
            else if (funcAssigned == RIGHT)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonRight);
                std::cout << " found assign!!! " << RIGHT << std::endl;
            }
            else if (funcAssigned == ENTER)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonEnter);
                std::cout << " found assign!!! " << ENTER << std::endl;
            }
            else if (funcAssigned == ESC)
            {
                bl.subscribeOnPin(shiftAssigned, callbackButtonEsc);
                std::cout << " found assign!!! " << ESC << std::endl;
            }
            else
            {
                std::cout << " No FUNC found for " << funcAssigned.c_str() << std::endl;
            }
        }
    }
    menuProcessor.initActiveMenuItem();

    bl.run();
    std::cout << "Hello, Buttons" << std::endl;
    while(1)
    {}
    return 0;
}

<<<<<<< HEAD
//void buttonUp(MenuProcessor &menuProcessor)
//{
//    std::cout << "Button UP pressed 2" << std::endl;

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
=======
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
>>>>>>> parent of 076a533... New class menuprocessor added to project
