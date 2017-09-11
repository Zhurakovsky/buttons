#include <iostream>
#include "buttonListener.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"
#include "menubuilder.hpp"
#include "displayoled.hpp"
//#include "menuprocessor.hpp"

//#include <deque>
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
void printMenuItemsRow(const std::vector<MenuItem*> &menuItems);
uint32_t getActiveId(const std::vector<MenuItem*> &menu);
uint32_t getNeighbourId(const std::vector<MenuItem*> &menu, const MenuFindDirection &findDirection);
void setNeighbourActive(const std::vector<MenuItem*> &menu, const MenuFindDirection &findDirection);
MenuItem* getItemById(const std::vector<MenuItem*> &menu, uint32_t itemId);
std::vector<MenuItem*> getItemsRow(MenuItem *baseItem);

int main()
{
    ButtonListener bl;
    std::vector<MenuItem*> menu;
    DisplayOled displ;
    displ.init();
    displ.initTest();
    //displ.close();
    //return 0;

    //Build menu
    MenuBuilder mBuilder;
    mBuilder.buildMenu(configFile, menu);

    std::cout << "Build menu OK" << std::endl;

    // Assign input GPIO PINS according to config
    // First argument - phisically pin number on RPi
    // Secong - bcm2835 shift value
    std::map<int, int> mapPinGpio;
    mBuilder.buildPinGpioMap(configFile, mapPinGpio);

    std::cout << "Build PIN GPIO Map OK" << std::endl;

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
        //printMenu(menu);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.printMenuList(activeItemRow);
    };
    std::function<void()> callbackButtonDown = [&]()
    {
        std::cout << "Button DOWN pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::CHILD;
        setNeighbourActive(menu, findDirection);
        //printMenu(menu);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.printMenuList(activeItemRow);
     };
    std::function<void()> callbackButtonLeft = [&]()
    {
        std::cout << "Button LEFT pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::LEFT;
        setNeighbourActive(menu, findDirection);
        //printMenu(menu);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.printMenuList(activeItemRow);
    };
    std::function<void()> callbackButtonRight = [&]()
    {
        std::cout << "Button RIGHT pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::RIGHT;
        setNeighbourActive(menu, findDirection);
        //printMenu(menu);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.printMenuList(activeItemRow);
    };
    std::function<void()> callbackButtonEnter = [&]()
    {
        std::cout << "Button ENTER pressed" << std::endl;
    };
    std::function<void()> callbackButtonEsc = [&]()
    {
        std::cout << "Button ESC pressed" << std::endl;
    };

    std::cout << "Callbacks assigned OK" << std::endl;

    std::map<int, std::string> mapButtonsFuncAssigned;
    mBuilder.buildButtonsFuncAssigned(configFile, mapButtonsFuncAssigned);

    std::cout << "Button funcs assigned OK" << std::endl;

    //std::cout << "Size of mapButtonsFuncAssigned map == " << mapButtonsFuncAssigned.size() << std::endl;

//    for (auto it = mapButtonsFuncAssigned.begin(); it != mapButtonsFuncAssigned.end(); ++it)
//    {
//        int intValue = it->first;
//        std::string tmpString = it->second;
//        std::cout << intValue << " " << tmpString.c_str() << std::endl;
//    }

    for (auto it = mapButtonsFuncAssigned.begin(); it != mapButtonsFuncAssigned.end(); ++it)
    {
        uint32_t pinAssigned = it->first;
        uint32_t purePinAssigned = 0;
        purePinAssigned |= (pinAssigned & 0x00FF);
        std::string withPullUp = ((pinAssigned == purePinAssigned) ? " with pull-down" : " with pull-up");
        const std::string funcAssigned(it->second);

        std::cout << purePinAssigned << " pin assigned to func "
                  << funcAssigned.c_str()
                  << withPullUp.c_str()
                  << std::endl;

        auto iter = mapPinGpio.find(purePinAssigned);
        if (iter != mapPinGpio.end())
        {
            uint32_t shiftAssigned = iter->second;
            if (pinAssigned != purePinAssigned) // if pull-up case
            {
                shiftAssigned |= (1 << 8); // add 1 to position 9 as pull-up marker
            }
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
    std::cout << "FUnc Subscription OK" << std::endl;

    //printMenu(menu);

    bl.run();

    std::cout << "Bl Run OK" << std::endl;

    while(1)
    {
        char c = getchar();
        if (c == 'q')
        {
            break;
        }
    }
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

MenuItem* getItemById(const std::vector<MenuItem*> &menu, uint32_t itemId)
{
    MenuItem *tmpItem = NULL;
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        tmpItem = *it;

        if (tmpItem)
        {
            if (tmpItem->getItemId() == itemId)
            {
                break;
            }
        }
    }
    return tmpItem;
}

std::vector<MenuItem*> getItemsRow(MenuItem* baseItem)
{
    //std::deque<MenuItem*>myDeque;
    MenuItem* centerItem = baseItem;
    std::vector<MenuItem*> v;
    //myDeque.push_front(centerItem);
    MenuItem* tmpItem = centerItem;

    while(tmpItem->getPrevious())
    {
        tmpItem = tmpItem->getPrevious();
//        if (tmpItem)
//        {
//            myDeque.push_front(tmpItem);
//        }
    }
    v.push_back(tmpItem);
    while(tmpItem)
    {
        tmpItem = tmpItem->getNext();
        if (tmpItem)
        {
            v.push_back(tmpItem);
        }
    }


//    for (auto itt = myDeque.begin(); itt != myDeque.end(); ++itt)
//    {
//        MenuItem* tmpitt = *itt;
//        v.push_back(tmpitt);
//    }
    return v;
}

void printMenuItemsRow(const std::vector<MenuItem*> &menuItems)
{
    for (auto it = menuItems.begin(); it != menuItems.end(); ++it)
    {
        MenuItem* tmpItem = *it;
        if (tmpItem)
        {
            bool activeFlag = tmpItem->isActive();
            std::string itemName = tmpItem->itemName();
            std::cout << itemName.c_str() << (activeFlag ? " <<<" : "") << std::endl;
        }
    }
}
