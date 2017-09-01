#include <iostream>
#include "buttonListener.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"
#include "menubuilder.hpp"
//#include "menuprocessor.hpp"

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

void printMenu(const std::vector<MenuItem*> &menu);
uint32_t getActiveId(const std::vector<MenuItem*> &menu);
uint32_t getNeighbourId(const std::vector<MenuItem*> &menu, const MenuFindDirection &findDirection);
void setNeighbourActive(const std::vector<MenuItem*> &menu, const MenuFindDirection &findDirection);

int main()
{
    ButtonListener bl;
    std::vector<MenuItem*> menu;

    //Build menu
    MenuBuilder mBuilder;
    mBuilder.buildMenu(configFile, menu);

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
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::PARENT;
        setNeighbourActive(menu, findDirection);
        printMenu(menu);
    };
    std::function<void()> callbackButtonDown = [&]()
    {
        std::cout << "Button DOWN pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::CHILD;
        setNeighbourActive(menu, findDirection);
        printMenu(menu);
     };
    std::function<void()> callbackButtonLeft = [&]()
    {
        std::cout << "Button LEFT pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::LEFT;
        setNeighbourActive(menu, findDirection);
        printMenu(menu);
    };
    std::function<void()> callbackButtonRight = [&]()
    {
        std::cout << "Button RIGHT pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::RIGHT;
        setNeighbourActive(menu, findDirection);
        printMenu(menu);
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

    //printMenu(menu);

    bl.run();
    while(1)
    {}
    return 0;
}

void printMenu(const std::vector<MenuItem*> &menu)
{
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *thisItem = *it;
        MenuItem *parentItem = thisItem->getParent();
        MenuItem *childItem = thisItem->getChild();
        MenuItem *leftItem = thisItem->getPrevious();
        MenuItem *rightItem = thisItem->getNext();

        uint32_t parentItemId;
        uint32_t thisItemId;
        uint32_t childItemId;
        uint32_t leftItemId;
        uint32_t rightItemId;

        bool currentActive = thisItem->isActive();

        if (parentItem)
        {
            parentItemId = parentItem->getItemId();
        }
        else
        {
            parentItemId = 0;
        }

        thisItemId = thisItem->getItemId();

        if (childItem)
        {
            childItemId = childItem->getItemId();
        }
        else
        {
            childItemId = 0;
        }

        if (leftItem)
        {
            leftItemId = leftItem->getItemId();
        }
        else
        {
            leftItemId = 0;
        }

        if (rightItem)
        {
            rightItemId = rightItem->getItemId();
        }
        else
        {
            rightItemId = 0;
        }

        std::cout << "*** \t *** \t ***" << std::endl;
        std::cout << "\t" << parentItemId << std::endl;
        if (currentActive)
        {
            std::cout << leftItemId <<"\t>" << thisItemId << "<\t" << rightItemId << std::endl;
        }
        else
        {
            std::cout << leftItemId <<"\t" << thisItemId << "\t" << rightItemId << std::endl;
        }
        std::cout << "\t" << childItemId << std::endl;
    }

}

uint32_t getActiveId(const std::vector<MenuItem*> &menu)
{
    uint32_t res = 0;
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *tmpItem = *it;
        if (tmpItem)
        {
            if (tmpItem->isActive())
            {
                res = tmpItem->getItemId();
            }
        }
    }
    return res;
}

uint32_t getNeighbourId(const std::vector<MenuItem*> &menu, const MenuFindDirection &findDirection)
{
    uint32_t res = 0;
    uint32_t activeId = getActiveId(menu);
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *tmpItem = *it;
        if (tmpItem)
        {
            if(tmpItem->getItemId() == activeId)
            {
                if (findDirection == rpibuttons::MenuFindDirection::PARENT)
                {
                    MenuItem *neighbourItem = tmpItem->getParent();
                    if (neighbourItem)
                    {
                        res = neighbourItem->getItemId();
                    }
                }
                else if (findDirection == rpibuttons::MenuFindDirection::CHILD)
                {
                    MenuItem *neighbourItem = tmpItem->getChild();
                    if (neighbourItem)
                    {
                        res = neighbourItem->getItemId();
                    }
                }
                else if (findDirection == rpibuttons::MenuFindDirection::LEFT)
                {
                    MenuItem *neighbourItem = tmpItem->getPrevious();
                    if (neighbourItem)
                    {
                        res = neighbourItem->getItemId();
                    }
                }
                else if (findDirection == rpibuttons::MenuFindDirection::RIGHT)
                {
                    MenuItem *neighbourItem = tmpItem->getNext();
                    if (neighbourItem)
                    {
                        res = neighbourItem->getItemId();
                    }
                }
                else if (findDirection == rpibuttons::MenuFindDirection::CURRENT)
            {
                res = tmpItem->getItemId();
            }
            }
        }
    }
    return res;
}

void setNeighbourActive(const std::vector<MenuItem*> &menu, const MenuFindDirection &findDirection)
{
    uint32_t activeId = getActiveId(menu);
    uint32_t neighbourId = getNeighbourId(menu, findDirection);
    bool activeSet = false;
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *tmpItem = *it;
        if (tmpItem)
        {
            uint32_t tmpId = tmpItem->getItemId();
            if (tmpId == neighbourId)
            {
                tmpItem->setActive(true);
                activeSet = true;
            }
        }
    }
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *tmpItem = *it;
        if (tmpItem)
        {
            uint32_t tmpId = tmpItem->getItemId();
            if ((tmpId == activeId) && (activeSet == true))
            {
                tmpItem->setActive(false);
            }
        }
    }
}

