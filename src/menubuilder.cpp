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
}
