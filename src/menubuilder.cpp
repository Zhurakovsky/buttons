#include "menubuilder.hpp"
#include <sstream>
#include <vector>
#include <iterator>
#include <map>


namespace rpibuttons
{

void MenuBuilder::buildMenu(const std::string &fileName, std::vector<MenuItem*> &menu)
{
    const std::string searchPattern = "M";
    std::vector<std::string> menuList;
    std::vector<menuParserString> menuItemParams;
    menuList = confParser.getConfigStrings(fileName, searchPattern);

    for(auto it = menuList.begin(); it != menuList.end(); ++it )
    {
        std::string tmpString = *it;
        std::istringstream iss(tmpString);

        menuParserString mps;
        std::string tmpM;
        iss >> tmpM
                >> mps.itemId
                >> mps.parentId
                >> mps.leftItemId
                >> mps.rightItemId
                >> mps.itemCaption
                >> mps.itemActionType
                >> mps.itemActionParameter;
        menuItemParams.push_back(mps);
        MenuItem *mi = new MenuItem(mps.itemId);
        mi->setItemName(mps.itemCaption);

        MenuItemActionType miat;
        MenuItemActionProperties miaprop;
        if (mps.itemActionType == "MenuDropdown")
        {
            miat = MenuItemActionType::MenuDropdown;
            miaprop.childMenuId = mps.itemActionParameter;
        }
        else if (mps.itemActionType == "DisplayText")
        {
            miat = MenuItemActionType::DisplayText;
            miaprop.textToShow = mps.itemActionParameter;
        }
        else if (mps.itemActionType == "DisplayGraphics")
        {
            miat = MenuItemActionType::DisplayGraphics;
            miaprop.pathToGraphics = mps.itemActionParameter;
        }
        else if (mps.itemActionType == "PlayVideo")
        {
            miat = MenuItemActionType::PlayVideo;
            miaprop.pathToVideoFile = mps.itemActionParameter;
        }
        else if (mps.itemActionType == "RunProgram")
        {
            miat = MenuItemActionType::RunProgram;
            miaprop.pathToApplication = mps.itemActionParameter;
        }
        else if (mps.itemActionType == "ClearScreen")
        {
            miat = MenuItemActionType::ClearScreen;
            std::istringstream screenRectIss(mps.itemActionParameter);
            ScreenRect sr;
            screenRectIss >> sr.x >> sr.y >> sr.w >> sr.h;
            miaprop.setScreenRect(sr);
        }
        else
        {
            miat = MenuItemActionType::WrongActionType;
        }

         mi->setActionType(miat);
        mi->setMenuItemProperties(miaprop);

        menu.push_back(mi);

        //std::cout << "Created params for item " << mps.itemId << std::endl;
    }
    /*
     * Iterate through menu vector and set appropriate
     * pointers for "UP", "DOWN", "LEFT", "RIGHT" neighbours
     */
    for (auto it = menuItemParams.begin(); it != menuItemParams.end(); ++it)
    {
        struct menuParserString menuItemParam = *it;
        int itemId = menuItemParam.itemId;
        int parentId = menuItemParam.parentId;
        int leftItemId = menuItemParam.leftItemId;
        int rightItemId = menuItemParam.rightItemId;


        for (auto menuitemIter = menu.begin(); menuitemIter != menu.end(); ++menuitemIter)
        {
            //std::cout << itemId << " " << parentId << " " << leftItemId << " " << rightItemId << std::endl;
            MenuItem *processedMenuItem = *menuitemIter;
            uint32_t processedItemId = processedMenuItem->getItemId();

            if (processedItemId == static_cast<uint32_t>(itemId))
            {
                setParentItem(menu, processedMenuItem, parentId);
                setLeftItem(menu, processedMenuItem, leftItemId);
                setRightItem(menu, processedMenuItem, rightItemId);
            }
        }
    }

    //Define first item of menu vector as active item
    if (menu.size() > 0)
    {
        for(auto it = menu.begin(); it != menu.end(); ++it)
        {
            MenuItem *menuItem = *it;
            menuItem->setActive(false);
        }
        auto it = menu.begin();
        MenuItem *menuItem = *it;
        menuItem->setActive(true);


    }
    //int menuItemCount = menuItemParams.size();
    //std::cout << "/nCreated " << menuItemCount << " menu items" << std::endl;
    for(auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *menuItem = *it;
        std::string itemName = menuItem->itemName();
        std::cout << itemName.c_str() << ", ";
        std::cout << ((menuItem->isActive()) ? "Active" : "Passive") << std::endl;
    }
}

void MenuBuilder::buildPinGpioMap(const std::string &fileName, std::map<int, int> &mapPinGpio)
{
    const std::string searchPattern = "G";
    std::vector<std::string> pinList;

    pinList = confParser.getConfigStrings(fileName, searchPattern);

    for(auto it = pinList.begin(); it != pinList.end(); ++it )
    {
        std::string tmpString = *it;
        std::istringstream iss(tmpString);
        int pinNumber;
        int pinDescription;

        std::string tmpG;
        iss >> tmpG >> pinNumber >> pinDescription;

        mapPinGpio.emplace(std::make_pair(pinNumber, pinDescription));
    }
}

void MenuBuilder::buildButtonsFuncAssigned(const std::string &fileName, std::map<int, std::string> &mapButtonsFuncAssigned)
{
    const std::string searchPattern = "I";
    std::vector<std::string> funcList;

    funcList = confParser.getConfigStrings(fileName, searchPattern);

    for(auto it = funcList.begin(); it != funcList.end(); ++it )
    {
        std::string tmpString = *it;
        std::istringstream iss(tmpString);
        uint32_t pinNumber;
        uint32_t pullUpDown = 0; //read pull info from config string
        std::string funcAssigned;

        std::string tmpI;
        iss >> tmpI >> pinNumber >> funcAssigned >> pullUpDown;

        if (pullUpDown == 1)
        {
            pinNumber |= (pullUpDown << 8);
        }

        mapButtonsFuncAssigned.emplace(std::make_pair(pinNumber, funcAssigned));

    }
}

void MenuBuilder::setParentItem(std::vector<MenuItem*> &menu, MenuItem *processedMenuItem, uint32_t parentId)
{
    // By default menu item already have nullptr as parent
    if (parentId == 0)
    {
        return;
    }

    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *testedItem = *it;
        uint32_t testedParent = testedItem->getItemId();
        if (parentId == testedParent)
        {
            if (processedMenuItem->getParent() == NULL)
            {
                processedMenuItem->setParent(testedItem);
            }
            if (testedItem->getChild() == NULL)
            {
                testedItem->setChild(processedMenuItem);
            }
        }
    }
}

void MenuBuilder::setLeftItem(std::vector<MenuItem*> &menu, MenuItem *processedMenuItem, uint32_t leftId)
{

    // By default menu item already have nullptr as previous
    if (leftId == 0)
    {
        return;
    }

    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *testedItem = *it;
        uint32_t testedLeft = testedItem->getItemId();
        if (leftId == testedLeft)
        {
            //In our context we consider Left as Previous
            if (processedMenuItem->getPrevious() == NULL)
            {
                processedMenuItem->setPrevious(testedItem);
            }
            if (testedItem->getNext() == NULL)
            {
                testedItem->setNext(processedMenuItem);
            }
        }
    }
}

void MenuBuilder::setRightItem(std::vector<MenuItem*> &menu, MenuItem *processedMenuItem, uint32_t rightId)
{

    // By default menu item already have nullptr as next
    if (rightId == 0)
    {
        return;
    }

    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *testedItem = *it;
        uint32_t testedRight = testedItem->getItemId();
        if (rightId == testedRight)
        {
            //In our context we consider Left as Previous
            if (processedMenuItem->getNext() == NULL)
            {
                processedMenuItem->setNext(testedItem);
            }
            if (testedItem->getPrevious() == NULL)
            {
                testedItem->setPrevious(processedMenuItem);
            }
        }
    }
}
}
