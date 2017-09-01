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
        //int parentId = menuItemParam.parentId;
        //int leftItemId = menuItemParam.leftItemId;
        //int rightItemId = menuItemParam.rightItemId;


        for (auto menuitemIter = menu.begin(); menuitemIter != menu.end(); ++menuitemIter)
        {
            MenuItem *processedMenuItem = *menuitemIter;
            uint32_t processedItemId = processedMenuItem->getItemId();

            if (processedItemId == static_cast<uint32_t>(itemId))
            {
                //setParentItem(menu, processedMenuItem, parentId);
                //setLeftItem(menu, processedMenuItem, leftItemId);
                //setRightItem(menu, processedMenuItem, rightItemId);
            }
        }
    }

    //Define first item of menu vector as active item
    if (menu.size() > 0)
    {
        auto it = menu.begin();
        MenuItem *startActiveMenuItem = *it;
        startActiveMenuItem->setActive(true);
        ++it;
        // and all another items define as non-active
        for(; it != menu.end(); ++it)
        {
            MenuItem *menuItem = *it;
            menuItem->setActive(false);
        }
    }
    //int menuItemCount = menuItemParams.size();
    //std::cout << "/nCreated " << menuItemCount << " menu items" << std::endl;
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
        int pinNumber;
        std::string funcAssigned;

        std::string tmpI;
        iss >> tmpI >> pinNumber >> funcAssigned;

        mapButtonsFuncAssigned.emplace(std::make_pair(pinNumber, funcAssigned));

    }
}

//void MenuBuilder::setParentItem(std::vector<MenuItem*> &menu, MenuItem *processedMenuItem, uint32_t parentId)
//{
//    // By default menu item already have nullptr as parent
//    if (parentId == 0)
//    {
//        return;
//    }

//    for (auto it = menu.begin(); it != menu.end(); ++it)
//    {
//        MenuItem *testedItem = *it;
//        uint32_t testedParent = testedItem->getItemId();
//        if (parentId == testedParent)
//        {
//            if (processedMenuItem->getParent() == nullptr)
//            {
//                processedMenuItem->setParent(testedItem);
//            }
//            if (testedItem->getChild() == nullptr)
//            {
//                testedItem->setChild(processedMenuItem);
//            }
//        }
//    }
//}

//void MenuBuilder::setLeftItem(std::vector<MenuItem*> &menu, MenuItem *processedMenuItem, uint32_t leftId)
//{

//    // By default menu item already have nullptr as previous
//    if (leftId == 0)
//    {
//        return;
//    }

//    for (auto it = menu.begin(); it != menu.end(); ++it)
//    {
//        MenuItem *testedItem = *it;
//        uint32_t testedLeft = testedItem->getItemId();
//        if (leftId == testedLeft)
//        {
//            //In our context we consider Left as Previous
//            if (processedMenuItem->getPrevious() == nullptr)
//            {
//                processedMenuItem->setPrevious(testedItem);
//            }
//            if (testedItem->getNext() == nullptr)
//            {
//                testedItem->setNext(processedMenuItem);
//            }
//        }
//    }
//}

//void MenuBuilder::setRightItem(std::vector<MenuItem*> &menu, MenuItem *processedMenuItem, uint32_t rightId)
//{

//    // By default menu item already have nullptr as next
//    if (rightId == 0)
//    {
//        return;
//    }

//    for (auto it = menu.begin(); it != menu.end(); ++it)
//    {
//        MenuItem *testedItem = *it;
//        uint32_t testedRight = testedItem->getItemId();
//        if (rightId == testedRight)
//        {
//            //In our context we consider Left as Previous
//            if (processedMenuItem->getNext() == nullptr)
//            {
//                processedMenuItem->setNext(testedItem);
//            }
//            if (testedItem->getPrevious() == nullptr)
//            {
//                testedItem->setPrevious(processedMenuItem);
//            }
//        }
//    }
//}
}
