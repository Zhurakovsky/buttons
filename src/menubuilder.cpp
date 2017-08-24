#include "menubuilder.hpp"
#include <sstream>

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
        std::istringstream iss (*it);

        menuParserString mps;
        iss >> mps.itemId
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

        std::cout << "Created params for item " << mps.itemId <<std::endl;
    }
    //int menuItemCount = menuItemParams.size();
}


}
