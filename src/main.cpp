#include <iostream>
#include "buttonListener.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"
#include "menubuilder.hpp"
#include "displayoled.hpp"
//#include <stack>
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

//42x50

const unsigned char example_bmp[] =
{
0x2A, 0x00, 0x32, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0xDC, 0xDD, 0xDE, 0xDF, 0xDF,
0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
};

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
 //   std::stack<MenuItem*> historyStack;
    DisplayOled displ;
    displ.init();

    //Build menu
    MenuBuilder mBuilder;
    mBuilder.buildMenu(configFile, menu);
    printMenu(menu);
    // Assign input GPIO PINS according to config
    // First argument - phisically pin number on RPi
    // Secong - bcm2835 shift value
    std::map<int, int> mapPinGpio;
    mBuilder.buildPinGpioMap(configFile, mapPinGpio);
    std::function<void()> callbackButtonUp = [&]()
    {
        std::cout << "Button UP pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::PARENT;
        setNeighbourActive(menu, findDirection);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
  //      historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.resetCurrentActivePosition();
        displ.printMenuList(activeItemRow);
    };
    std::function<void()> callbackButtonDown = [&]()
    {
        std::cout << "Button DOWN pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::CHILD;
        setNeighbourActive(menu, findDirection);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
   //     historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.resetCurrentActivePosition();
        displ.printMenuList(activeItemRow);
     };
    std::function<void()> callbackButtonLeft = [&]()
    {
        std::cout << "Button LEFT pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::LEFT;
        setNeighbourActive(menu, findDirection);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
    //    historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.decreaseCurrentActivePosition(activeItemRow);
        displ.printMenuList(activeItemRow);
    };
    std::function<void()> callbackButtonRight = [&]()
    {
        std::cout << "Button RIGHT pressed" << std::endl;
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::RIGHT;
        setNeighbourActive(menu, findDirection);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
     //   historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.increaseCurrentActivePosition(activeItemRow);
        displ.printMenuList(activeItemRow);
    };
    std::function<void()> callbackButtonEnter = [&]()
    {
        std::cout << "Button ENTER pressed" << std::endl;
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
      //  historyStack.push(activeItem);
        MenuItemActionType actionType = activeItem->getActionType();
        MenuItemActionProperties prop = activeItem->getMenuItemProperties();

        if(actionType == MenuItemActionType::DisplayText)
        {
            std::string textForDisplay = prop.textToShow;
            displ.printText(textForDisplay);
        }
        else if (actionType == MenuItemActionType::DisplayGraphics)
        {
            //uint32_t activeItemId = getActiveId(menu);
            //MenuItem* activeItem = getItemById(menu, activeItemId);
      //      historyStack.push(activeItem);

            //std::vector<const uint8_t>tmpBitmap(prop.bitmap);

            uint8_t bitmapData* = prop.bitmap.data();
            //const uint8_t *testBitmap = tmpBitmap[0];
            //int i = 0;
            //for (; testBitmap[i] != '\0'; i++);

                uint16_t tW = prop.imageW;
                uint16_t tH = prop.imageH;
                std::cout << "Going to print bitmap size "
                          << " W = " << tW
                          << " H = " << tH << std::endl;
                displ.drawBitmap(0, 0, bitmapData, tW, tH, 1);


            //displ.drawBitmap(43, 7, example_bmp, 42, 50, 1);

        }
        else if (actionType == MenuItemActionType::RunProgram)
        {

        }
        else if (actionType == MenuItemActionType::ClearScreen)
        {
            displ.clear();
        }
        else if (actionType == MenuItemActionType::MenuDropdown)
        {
            uint32_t mId = (uint32_t)atoi(prop.childMenuId.c_str());

            MenuItem* activeItem = getItemById(menu, mId);
            activeItem->setActive(true);
      //      historyStack.push(activeItem);
            std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
            printMenuItemsRow(activeItemRow);
            displ.resetCurrentActivePosition();
            displ.printMenuList(activeItemRow);
        }
    };
    std::function<void()> callbackButtonEsc = [&]()
    {
        std::cout << "Button ESC pressed" << std::endl;


        std::cout << "Go to print menu" << std::endl;
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
        //historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        //printMenuItemsRow(activeItemRow);
        displ.resetCurrentActivePosition();
        displ.printMenuList(activeItemRow);
        /*
        if(!historyStack.empty())
        {
            MenuItem* activeItem = historyStack.top();
            historyStack.pop();
            activeItem->setActive(true);
            displ.resetCurrentActivePosition();
            displ.printMenuList(activeItemRow);
        }
        else
        {
            std::cout << "History empty" << std::endl;
            uint32_t activeItemId = getActiveId(menu);
            MenuItem* activeItem = getItemById(menu, activeItemId);
            //historyStack.push(activeItem);
            std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
            //printMenuItemsRow(activeItemRow);
            displ.resetCurrentActivePosition();
            displ.printMenuList(activeItemRow);
        }
        */
    };

    std::map<int, std::string> mapButtonsFuncAssigned;
    mBuilder.buildButtonsFuncAssigned(configFile, mapButtonsFuncAssigned);
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

    bl.run();
    uint32_t actItemId = getActiveId(menu);
    MenuItem* acItem = getItemById(menu, actItemId);
 //   historyStack.push(activeItem);
    std::vector<MenuItem*> acItemRow = getItemsRow(acItem);
    //printMenuItemsRow(activeItemRow);
    displ.resetCurrentActivePosition();
    displ.printMenuList(acItemRow);
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
    MenuItem* centerItem = baseItem;
    std::vector<MenuItem*> v;
    MenuItem* tmpItem = centerItem;

    while(tmpItem->getPrevious())
    {
        tmpItem = tmpItem->getPrevious();
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

