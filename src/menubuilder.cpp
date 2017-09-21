#include "menubuilder.hpp"
#include <sstream>
#include <fstream>

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
    std::cout << "Read from config:" << std::endl;
    for (std::string listItem : menuList)
    {
        std::cout << listItem.c_str() << std::endl;
    }

    for(auto it = menuList.begin(); it != menuList.end(); ++it )
    {
        std::string tmpString = *it;
        std::size_t foundFirst = tmpString.find_first_of("\"");
        std::size_t foundLast = tmpString.find_last_of("\"");
        std::string paramString = tmpString.substr(foundFirst + 1, (foundLast - foundFirst - 1));
        tmpString = tmpString.substr(0, (foundFirst-1));

        std::cout << "Param String: " << paramString.c_str() << std::endl;
        std::cout << "Work String: " << tmpString.c_str() << std::endl;

        std::istringstream iss(tmpString);

        menuParserString mps;
        std::string tmpM;
        iss >> tmpM
                >> mps.itemId
                >> mps.parentId
                >> mps.leftItemId
                >> mps.rightItemId
                >> mps.itemCaption
                >> mps.itemActionType;
        std::cout << " aaaaaaaaaaaaa Param string:" << paramString.c_str()  << std::endl;
        mps.itemActionParameter.append(paramString);
        menuItemParams.push_back(mps);
        MenuItem *mi = new MenuItem(mps.itemId);
        mi->setItemName(mps.itemCaption);


        std::cout << "Parsed item parameters:" << std::endl;
        std::cout << "itemId:" << mps.itemId << std::endl;
        std::cout << "parentId:" << mps.parentId << std::endl;
        std::cout << "leftId:" << mps.leftItemId << std::endl;
        std::cout << "rightId:" << mps.rightItemId << std::endl;

        std::cout << "Caption:" << mps.itemCaption.c_str() << std::endl;
        std::cout << "ActionType:" << mps.itemActionType.c_str() << std::endl;
        std::cout << "Action parameter:" << mps.itemActionParameter.c_str() << std::endl;


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
            std::string rawGraphicProperty = mps.itemActionParameter;
            std::vector<std::string> displayImageParts;

            std::istringstream ss(rawGraphicProperty);
            while (ss)
            {
                std::string s;
              if (!getline( ss, s, ',' )) break;
              displayImageParts.push_back(s);
            }

            std::string graphicFilename;

            if (displayImageParts.size() == 3)
            {
                graphicFilename = displayImageParts[0];
                std::string value1 = displayImageParts.at(1);
                std::string value2 = displayImageParts.at(2);
                uint16_t bitmapW = (uint16_t)std::stoi(value1, nullptr, 10);
                uint16_t bitmapH = (uint16_t)std::stoi(value2, nullptr, 10);

                miaprop.pathToGraphics = graphicFilename;
                miaprop.imageW = bitmapW;
                miaprop.imageH = bitmapH;
            }
            else
            {
                return;
            }

            graphicFilename.append(".h");

            std::ifstream inputFile(graphicFilename.c_str());
            std::vector<std::string> records;
            std::vector<uint8_t> bitmapper;

            if (inputFile.is_open())
            {
                std::cout << "File open" << std::endl;
                while (!inputFile.eof())
                {
                    std::string line;
                    getline(inputFile, line);
                    if (line[0] != '0')
                    {
                        continue;
                    }

                    std::istringstream sss(line);

                    while (sss)
                    {
                      std::string st;
                      if (!getline( sss, st, ',' )) break;
                      records.push_back(st);
                    }
                }
                std::cout << "Loaded values:" << records.size() << std::endl;
                for(std::string line : records)
                {
                    uint8_t number = (uint8_t)strtol(line.c_str(), NULL, 0);
                    bitmapper.push_back(number);
                }
                inputFile.close();
            }
            else
            {
                std::cout << "Outer bitmap file " << graphicFilename.c_str() << " open ERROR" << std::endl;
            }
            std::cout << "Vector bitmapper has  " << bitmapper.size() << " enties" << std::endl;
            //const uint8_t* pictureBitmap = bitmapper.data();
            for (uint8_t bm : bitmapper)
            {
                miaprop.bitmap.push_back(bm);
            }
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

std::string MenuBuilder::trim(const std::string& str, const std::string& whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
}
