#include <iostream>
#include "buttonListener.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"
#include "menubuilder.hpp"
#include "displayoled.hpp"
#include "ILogger.hpp"

#include <vector>
#include <map>
#include <iterator>
#include <functional>
#include <iterator>
#include <string>
#include <termios.h>

//#include <mutex>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>

#include <bcm2835.h>

const std::string configFile = "config.txt";

using namespace std;
using namespace rpibuttons;
using namespace logspace;

class BufferToggle
{
    private:
        struct termios t;

    public:
        /* Disables buffered input */
        void off(void)
        {
            tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
            t.c_lflag &= ~ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(STDIN_FILENO, TCSANOW, &t); //Apply the new settings

        }

        /* Enables buffered input */
        void on(void)
        {
            tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
            t.c_lflag |= ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(STDIN_FILENO, TCSANOW, &t); //Apply the new settings
        }
};

void printMenu(const std::vector<MenuItem*> &menu);
void printMenuItemsRow(const std::vector<MenuItem*> &menuItems);
uint32_t getActiveId(const std::vector<MenuItem*> &menu);
uint32_t getNeighbourId(const std::vector<MenuItem*> &menu, const MenuFindDirection &findDirection);
void setNeighbourActive(const std::vector<MenuItem*> &menu, const MenuFindDirection &findDirection);
MenuItem* getItemById(const std::vector<MenuItem*> &menu, uint32_t itemId);
std::vector<MenuItem*> getItemsRow(MenuItem *baseItem);

void processButtonUp(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode);
void processButtonDown(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode);
void processButtonLeft(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode);
void processButtonRight(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode);
void processButtonEnter(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode);
void processButtonEsc(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode);

std::vector<std::string> getTextFromApplication(const std::string &pathToApp, const uint16_t &numberOfLines);

std::vector<std::string> getTextFromFile(const std::string &pathToFile, const uint16_t &numberOfLines);

int main()
{
    //std::cout << "[LOGS:Main]Step 1" << std::endl;
    std::string filename{"logfile.txt"};
    logspace::ILogger *logger = logspace::CreateLoggerInstance();
    logger->initLogger(filename);

    BufferToggle bt;
    ButtonListener bl;
    std::vector<MenuItem*> menu;
 //   std::stack<MenuItem*> historyStack;
    DisplayOled displ;
    displ.init();

    logger->writeLog("Step 2. After Display init");
    //std::cout << "[LOGS:Main]Step 2. After Display init" << std::endl;

    //mutable std::mutex main_mtx;

    rpibuttons::OledExecMode oledMode = rpibuttons::OledExecMode::MENU_MODE;

    //Build menu
    MenuBuilder mBuilder;
    mBuilder.buildMenu(configFile, menu);

    logger->writeLog("Step 3. After Menu complete");

    //std::cout << "[LOGS:Main]Step 3. After Menu complete" << std::endl;

    //TerminalMenu terMenu(displ, menu);
    //printMenu(menu);
    // Assign input GPIO PINS according to config
    // First argument - phisically pin number on RPi
    // Secong - bcm2835 shift value
    std::map<int, int> mapPinGpio;
    mBuilder.buildPinGpioMap(configFile, mapPinGpio);

    //std::cout << "[LOGS:Main]Step 4. After pins assigned" << std::endl;
    logger->writeLog("4. After pins assigned");
    std::function<void()> callbackButtonUp = [&]()
    {
        std::cout << "Button UP pressed" << std::endl;

        if ( oledMode == rpibuttons::OledExecMode::MENU_MODE)
        {
            MenuFindDirection findDirection = rpibuttons::MenuFindDirection::PARENT;
            setNeighbourActive(menu, findDirection);
            uint32_t activeItemId = getActiveId(menu);
            MenuItem* activeItem = getItemById(menu, activeItemId);
      //      historyStack.push(activeItem);
            std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
            printMenuItemsRow(activeItemRow);
            displ.resetCurrentActivePosition();
            displ.printMenuList(activeItemRow);
        }
        else if (oledMode == rpibuttons::OledExecMode::GRAPHICS_MODE)
        {
            displ.drawBitmap(0, -1);
        }
    };

    std::function<void()> callbackButtonDown = [&]()
    {
        std::cout << "Button DOWN pressed" << std::endl;
        if ( oledMode == rpibuttons::OledExecMode::MENU_MODE)
        {
            MenuFindDirection findDirection = rpibuttons::MenuFindDirection::CHILD;
            setNeighbourActive(menu, findDirection);
            uint32_t activeItemId = getActiveId(menu);
            MenuItem* activeItem = getItemById(menu, activeItemId);
       //     historyStack.push(activeItem);
            std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
            printMenuItemsRow(activeItemRow);
            displ.resetCurrentActivePosition();
            displ.printMenuList(activeItemRow);
        }
        else if (oledMode == rpibuttons::OledExecMode::GRAPHICS_MODE)
        {
            displ.drawBitmap(0, 1);
        }
     };
    std::function<void()> callbackButtonLeft = [&]()
    {
        std::cout << "Button LEFT pressed" << std::endl;
        if ( oledMode == rpibuttons::OledExecMode::MENU_MODE)
        {
            MenuFindDirection findDirection = rpibuttons::MenuFindDirection::LEFT;
            setNeighbourActive(menu, findDirection);
            uint32_t activeItemId = getActiveId(menu);
            MenuItem* activeItem = getItemById(menu, activeItemId);
        //    historyStack.push(activeItem);
            std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
            printMenuItemsRow(activeItemRow);
            displ.decreaseCurrentActivePosition(activeItemRow);
            displ.printMenuList(activeItemRow);
        }
        else if (oledMode == rpibuttons::OledExecMode::GRAPHICS_MODE)
        {
            displ.drawBitmap(-1, 0);
        }
    };
    std::function<void()> callbackButtonRight = [&]()
    {
        std::cout << "Button RIGHT pressed" << std::endl;
        if ( oledMode == rpibuttons::OledExecMode::MENU_MODE)
        {
            MenuFindDirection findDirection = rpibuttons::MenuFindDirection::RIGHT;
            setNeighbourActive(menu, findDirection);
            uint32_t activeItemId = getActiveId(menu);
            MenuItem* activeItem = getItemById(menu, activeItemId);
         //   historyStack.push(activeItem);
            std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
            printMenuItemsRow(activeItemRow);
            displ.increaseCurrentActivePosition(activeItemRow);
            displ.printMenuList(activeItemRow);
        }
        else if (oledMode == rpibuttons::OledExecMode::GRAPHICS_MODE)
        {
            displ.drawBitmap(1, 0);
        }
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
            displ.setTextSize(prop.textSize);
            uint16_t numberOfLines = prop.textLineCount;
            std::vector<std::string> textToPrint;

            if (prop.textType == rpibuttons::TextDisplayType::APP_TYPE)
            {
                std::string pathToApp = prop.pathToApplication;
                textToPrint = getTextFromApplication(pathToApp, numberOfLines);
                if (textToPrint.size() > 0)
                {
                    displ.printText(textToPrint);
                }
                else
                {
                    //TODO: Log error
                    std::string textForDisplay = "ErrorReadApp";
                    displ.printText(textForDisplay);
                }
            }
            else if (prop.textType == rpibuttons::TextDisplayType::FILE_TYPE)
            {
                std::string pathToFile = prop.pathToTextFile;
                textToPrint = getTextFromFile(pathToFile, numberOfLines);

                if (textToPrint.size() > 0)
                {
//                    std::cout << "Will be printed:" << std::endl;
//                    for (std::string it : textToPrint)
//                    {
//                        std::cout << it.c_str() << std::endl;
//                    }
                    displ.printText(textToPrint);
                }
                else
                {
                    //TODO: Log error
                    logger->writeLog("Nothing to print. Error reading file");
                    //std::cout << "Nothing to print. Error reading file" << std::endl;
                    std::string textForDisplay = "ErrorReadFile";
                    displ.printText(textForDisplay);
                }
            }
            else if (prop.textType == rpibuttons::TextDisplayType::TEXT_TYPE)
            {
                std::string textForDisplay = prop.textToShow;
                //std::cout << "Will be printed: " << textForDisplay.c_str() << std::endl;
                displ.printText(textForDisplay);
            }
            else if (prop.textType == rpibuttons::TextDisplayType::ERROR_TYPE)
            {
                //TODO:: Print logs output about error
                std::string textForDisplay = prop.textToShow;
                displ.printText(textForDisplay);
            }
        }
        else if (actionType == MenuItemActionType::DisplayGraphics)
        {
            oledMode = rpibuttons::OledExecMode::GRAPHICS_MODE;
            displ.setTextSize(2);
            displ.drawBitmap((128 - prop.imageW)/2,
                             (64 - prop.imageH)/2,
                             prop.bitmap,
                             prop.imageW,
                             prop.imageH,
                             1);
        }
        else if (actionType == MenuItemActionType::RunProgram)
        {
            oledMode = rpibuttons::OledExecMode::EXEC_MODE;

            std::string pathToApplication = "sudo ./testapp/" + prop.pathToApplication + " 4 100 &";

            //std::cout << "Checking if processor is available..." << std::endl;
            if (system(NULL))
             {
                 puts ("Ok");
             }
             else
             {
                logger->writeLog("ERROR System call");
                puts ("ERROR System call");
                 exit (EXIT_FAILURE);
             }
            std::system(pathToApplication.c_str());
        }
        else if (actionType == MenuItemActionType::ClearScreen)
        {
            displ.clear();
        }
        else if (actionType == MenuItemActionType::MenuDropdown)
        {
            oledMode = rpibuttons::OledExecMode::MENU_MODE;

            uint32_t mId = (uint32_t)atoi(prop.childMenuId.c_str());

            MenuItem* activeItem = getItemById(menu, mId);
            activeItem->setActive(true);
      //    historyStack.push(activeItem);
            std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
            printMenuItemsRow(activeItemRow);
            displ.resetCurrentActivePosition();
            displ.printMenuList(activeItemRow);
        }
    };

    std::function<void()> callbackButtonEsc = [&]()
    {
        std::cout << "Button ESC pressed" << std::endl;

        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
        //historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.setTextSize(2);
        displ.resetCurrentActivePosition();
        displ.printMenuList(activeItemRow);

        oledMode = rpibuttons::OledExecMode::MENU_MODE;
    };

    //std::cout << "[LOGS:Main]Step 5. After callbacks assigned" << std::endl;
    logger->writeLog("Step 5. After callbacks assigned");
    std::map<int, std::string> mapButtonsFuncAssigned;
    mBuilder.buildButtonsFuncAssigned(configFile, mapButtonsFuncAssigned);
    for (auto it = mapButtonsFuncAssigned.begin(); it != mapButtonsFuncAssigned.end(); ++it)
    {
        uint32_t pinAssigned = it->first;
        uint32_t purePinAssigned = 0;
        purePinAssigned |= (pinAssigned & 0x00FF);
//        std::string withPullUp = ((pinAssigned == purePinAssigned) ? " with pull-down" : " with pull-up");
        const std::string funcAssigned(it->second);

//        std::cout << purePinAssigned << " pin assigned to func "
//                  << funcAssigned.c_str()
//                  << withPullUp.c_str()
//                  << std::endl;

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
    logger->writeLog("Step 6. After functions assigned and subscribed");
    //std::cout << "[LOGS:Main]Step 6. After functions assigned and subscribed" << std::endl;

    bl.run();
    logger->writeLog("Step 7. After runned");
    //std::cout << "[LOGS:Main]Step 7. After runned" << std::endl;
    uint32_t actItemId = getActiveId(menu);
    MenuItem* acItem = getItemById(menu, actItemId);
 // historyStack.push(activeItem);
    std::vector<MenuItem*> acItemRow = getItemsRow(acItem);
    printMenuItemsRow(acItemRow);
    displ.resetCurrentActivePosition();
    displ.printMenuList(acItemRow);
    bool m_bStillRead = true;
    while (m_bStillRead)
    {
        bt.off();
        char ch;
        do
        {
            ch = getchar();
            if (ch == 65)
            {
                std::cout << "You pressed UP key" << std::endl;
                processButtonUp(menu, displ, oledMode);
            }
            else if (ch == 66)
            {
                std::cout << "You pressed DOWN key" << std::endl;
                processButtonDown(menu, displ, oledMode);
            }
            else if (ch == 67)
            {
                std::cout << "You pressed RIGHT key" << std::endl;
                processButtonRight(menu, displ, oledMode);
            }
            else if (ch == 68)
            {
                std::cout << "You pressed LEFT key" << std::endl;
                processButtonLeft(menu, displ, oledMode);
            }
            else if (ch == '\n')
            {
                std::cout << "You pressed ENTER key" << std::endl;
                processButtonEnter(menu, displ, oledMode);
            }
            else if (ch == 27)
            {
                std::cout << "You pressed ESC key" << std::endl;
                processButtonEsc(menu, displ, oledMode);
            }
            else if (ch == 'q')
            {
                std::cout << "Exiting..." << std::endl;
                m_bStillRead = false;
                break;
            }
        }
        while(ch!='q');
        bt.on();

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


void processButtonUp(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode)
{
    std::cout << "Button UP pressed" << std::endl;

    if ( oledMode == rpibuttons::OledExecMode::MENU_MODE)
    {
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::PARENT;
        setNeighbourActive(menu, findDirection);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
  //      historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.resetCurrentActivePosition();
        displ.printMenuList(activeItemRow);
    }
    else if (oledMode == rpibuttons::OledExecMode::GRAPHICS_MODE)
    {
        displ.drawBitmap(0, -1);
    }
}

void processButtonDown(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode)
{
    std::cout << "Button DOWN pressed" << std::endl;
    if ( oledMode == rpibuttons::OledExecMode::MENU_MODE)
    {
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::CHILD;
        setNeighbourActive(menu, findDirection);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
   //     historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.resetCurrentActivePosition();
        displ.printMenuList(activeItemRow);
    }
    else if (oledMode == rpibuttons::OledExecMode::GRAPHICS_MODE)
    {
        displ.drawBitmap(0, 1);
    }
}

void processButtonLeft(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode)
{
    std::cout << "Button LEFT pressed" << std::endl;
    if ( oledMode == rpibuttons::OledExecMode::MENU_MODE)
    {
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::LEFT;
        setNeighbourActive(menu, findDirection);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
    //    historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.decreaseCurrentActivePosition(activeItemRow);
        displ.printMenuList(activeItemRow);
    }
    else if (oledMode == rpibuttons::OledExecMode::GRAPHICS_MODE)
    {
        displ.drawBitmap(-1, 0);
    }
}

void processButtonRight(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode)
{
    std::cout << "Button RIGHT pressed" << std::endl;
    if ( oledMode == rpibuttons::OledExecMode::MENU_MODE)
    {
        MenuFindDirection findDirection = rpibuttons::MenuFindDirection::RIGHT;
        setNeighbourActive(menu, findDirection);
        uint32_t activeItemId = getActiveId(menu);
        MenuItem* activeItem = getItemById(menu, activeItemId);
     //   historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.increaseCurrentActivePosition(activeItemRow);
        displ.printMenuList(activeItemRow);
    }
    else if (oledMode == rpibuttons::OledExecMode::GRAPHICS_MODE)
    {
        displ.drawBitmap(1, 0);
    }
}

void processButtonEnter(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode)
{
    std::cout << "Button ENTER pressed" << std::endl;
    uint32_t activeItemId = getActiveId(menu);
    MenuItem* activeItem = getItemById(menu, activeItemId);
  //  historyStack.push(activeItem);
    MenuItemActionType actionType = activeItem->getActionType();
    MenuItemActionProperties prop = activeItem->getMenuItemProperties();

    if(actionType == MenuItemActionType::DisplayText)
    {
        displ.setTextSize(prop.textSize);
        uint16_t numberOfLines = prop.textLineCount;
        std::vector<std::string> textToPrint;

        if (prop.textType == rpibuttons::TextDisplayType::APP_TYPE)
        {
            std::string pathToApp = prop.pathToApplication;
            textToPrint = getTextFromApplication(pathToApp, numberOfLines);
            if (textToPrint.size() > 0)
            {
                displ.printText(textToPrint);
            }
            else
            {
                //TODO: Log error
                std::string textForDisplay = "ErrorReadApp";
                //logspace::logger->writeLog("ErrorReadApp from config");
                displ.printText(textForDisplay);
            }
        }
        else if (prop.textType == rpibuttons::TextDisplayType::FILE_TYPE)
        {
            std::string pathToFile = prop.pathToTextFile;
            textToPrint = getTextFromFile(pathToFile, numberOfLines);
            if (textToPrint.size() > 0)
            {
                //std::cout << "Will be printed from file: " << std::endl;
//                for (std::string it : textToPrint)
//                {
//                    std::cout << it.c_str() << std::endl;
//                }
                displ.printText(textToPrint);
            }
            else
            {
                //TODO: Log error
                //std::cout << "Nothing to print. Error reading textfile" << std::endl;
                //logspace::logger->writeLog("Nothing to print. Error reading textfile");
                std::string textForDisplay = "ErrorReadFile";
                displ.printText(textForDisplay);
            }
        }
        else if (prop.textType == rpibuttons::TextDisplayType::TEXT_TYPE)
        {
            std::string textForDisplay = prop.textToShow;
            //std::cout << "Will be printed: " << textForDisplay.c_str() << std::endl;
            displ.printText(textForDisplay);
        }
        else if (prop.textType == rpibuttons::TextDisplayType::ERROR_TYPE)
        {
            //TODO:: Print logs output about error
            std::string textForDisplay = prop.textToShow;
            displ.printText(textForDisplay);
        }
    }
    else if (actionType == MenuItemActionType::DisplayGraphics)
    {
        oledMode = rpibuttons::OledExecMode::GRAPHICS_MODE;
        displ.setTextSize(2);
        displ.drawBitmap((128 - prop.imageW)/2,
                         (64 - prop.imageH)/2,
                         prop.bitmap,
                         prop.imageW,
                         prop.imageH,
                         1);
    }
    else if (actionType == MenuItemActionType::RunProgram)
    {
        oledMode = rpibuttons::OledExecMode::EXEC_MODE;

        std::string pathToApplication = "./testapp/" + prop.pathToApplication + " &";

        //std::cout << "Checking if processor is available..." << std::endl;
         if (system(NULL))
         {
             puts ("Ok");
         }
         else
         {
             puts ("ERROR System call");
             exit (EXIT_FAILURE);
         }
        std::system(pathToApplication.c_str());
    }
    else if (actionType == MenuItemActionType::ClearScreen)
    {
        displ.clear();
    }
    else if (actionType == MenuItemActionType::MenuDropdown)
    {
        oledMode = rpibuttons::OledExecMode::MENU_MODE;

        uint32_t mId = (uint32_t)atoi(prop.childMenuId.c_str());

        MenuItem* activeItem = getItemById(menu, mId);
        activeItem->setActive(true);
  //    historyStack.push(activeItem);
        std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
        printMenuItemsRow(activeItemRow);
        displ.resetCurrentActivePosition();
        displ.printMenuList(activeItemRow);
    }
}

void processButtonEsc(const std::vector<MenuItem*> &menu, DisplayOled &displ, rpibuttons::OledExecMode &oledMode)
{
    std::cout << "Button ESC pressed" << std::endl;

    uint32_t activeItemId = getActiveId(menu);
    MenuItem* activeItem = getItemById(menu, activeItemId);
    //historyStack.push(activeItem);
    std::vector<MenuItem*> activeItemRow = getItemsRow(activeItem);
    printMenuItemsRow(activeItemRow);
    displ.setTextSize(2);
    displ.resetCurrentActivePosition();
    displ.printMenuList(activeItemRow);

    oledMode = rpibuttons::OledExecMode::MENU_MODE;
}

std::vector<std::string> getTextFromApplication(const std::string &pathToApp, const uint16_t &numberOfLines)
{
    std::vector<std::string>textToPrint;

    FILE *fpipe;

    char *command = (char *)pathToApp.c_str();
    char line[256];

    if ( !(fpipe = (FILE*)popen(command, "r")))
    { // if fpipe is NULL
        perror("Problem with pipe");
        pclose(fpipe);
        return textToPrint;
        //exit(1);
    }

    uint16_t i = numberOfLines;
    while ((fgets(line, sizeof line, fpipe)) && (i--))
    {
        textToPrint.push_back(std::string(line));
    }
    pclose(fpipe);

    return textToPrint;
}

std::vector<std::string> getTextFromFile(const std::string &pathToFile, const uint16_t &numberOfLines)
{
    std::vector<std::string>textToPrint;

    std::string filename = pathToFile;
    std::ifstream source;
    source.open(filename);
    if (!source.is_open())
    {
        std::cout << "[LOGS::Main]getTextFromFile::Error opening file " << filename.c_str() << std::endl;
        return textToPrint;
    }
    std::string line;
    uint16_t i = numberOfLines;
    while ((std::getline(source, line)) && (i-- > 0))
    {
        textToPrint.push_back(line);
    }
    source.close();

    return textToPrint;
}
