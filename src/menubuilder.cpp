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
    std::cout << "Step 7" << std::endl;
    const std::string searchPattern = "M";
    std::vector<std::string> menuList;
    std::vector<menuParserString> menuItemParams;
    menuList = confParser.getConfigStrings(fileName, searchPattern);
std::cout << "Step 8" << std::endl;
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
            std::cout << "Step 9" << std::endl;
            miat = MenuItemActionType::MenuDropdown;
            miaprop.childMenuId = mps.itemActionParameter;
        }
        else if (mps.itemActionType == "DisplayText")
        {
            std::cout << "Step 10" << std::endl;
            miat = MenuItemActionType::DisplayText;
            miaprop.textToShow = mps.itemActionParameter;
        }
        else if (mps.itemActionType == "DisplayGraphics")
        {
            std::cout << "Step 11" << std::endl;
            miat = MenuItemActionType::DisplayGraphics;
            std::string rawGraphicProperty = mps.itemActionParameter;

            std::cout << "Step 11.01  rawGraphicProperty = " << rawGraphicProperty.c_str() << std::endl;
            std::vector<std::string> displayImageParts;
            std::istringstream ss(rawGraphicProperty);
            while (ss)
            {
              std::cout << "Step 11.02" << std::endl;
                std::string s;
              if (!getline( ss, s, ',' )) break;
              displayImageParts.push_back(s);
            }
            for (std::string myStr : displayImageParts)
            {
                std::cout << myStr << std::endl;
            }
            std::cout << "Step 11.1 displayImageParts.size == " << displayImageParts.size() << std::endl;
            std::string graphicFilename;
            if (graphicFilename.size() >= 3)
            {
            graphicFilename = displayImageParts[0];
            uint16_t bitmapW = (uint16_t)atoi(displayImageParts[1].c_str());
            uint16_t bitmapH = (uint16_t)atoi(displayImageParts[2].c_str());

            miaprop.pathToGraphics = graphicFilename;
            miaprop.imageW = bitmapW;
            miaprop.imageH = bitmapH;
            }
            else
            {
                std::cout << "Step 11.2 Return " << std::endl;
                return;
            }

            std::cout << "Step 11.3" << std::endl;

            graphicFilename.append(".h");
            std::cout << "Step 11.4 graphicFilename == " << graphicFilename.c_str() << std::endl;

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
            }
            inputFile.close();
            const uint8_t* pictureBitmap = bitmapper.data();
            miaprop.bitmap.emplace(miaprop.bitmap.end(), pictureBitmap);
        }
        else if (mps.itemActionType == "PlayVideo")
        {

            std::cout << "Step 13" << std::endl;
            miat = MenuItemActionType::PlayVideo;
            miaprop.pathToVideoFile = mps.itemActionParameter;
        }
        else if (mps.itemActionType == "RunProgram")
        {
            std::cout << "Step 14" << std::endl;
            miat = MenuItemActionType::RunProgram;
            miaprop.pathToApplication = mps.itemActionParameter;
        }
        else if (mps.itemActionType == "ClearScreen")
        {
            std::cout << "Step 15" << std::endl;
            miat = MenuItemActionType::ClearScreen;
            std::istringstream screenRectIss(mps.itemActionParameter);
            ScreenRect sr;
            screenRectIss >> sr.x >> sr.y >> sr.w >> sr.h;
            miaprop.setScreenRect(sr);
        }
        else
        {
            std::cout << "Step 16" << std::endl;
            miat = MenuItemActionType::WrongActionType;
        }

         mi->setActionType(miat);
         std::cout << "Step 17" << std::endl;
        mi->setMenuItemProperties(miaprop);
std::cout << "Step 18" << std::endl;
        menu.push_back(mi);
std::cout << "Step 19" << std::endl;
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
