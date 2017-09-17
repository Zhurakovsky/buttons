#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "configparser.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"


namespace rpibuttons
{
class MenuBuilder
{
public:
    void buildMenu(const std::string &fileName, std::vector<MenuItem*> &menu);

    void buildPinGpioMap(const std::string &fileName, std::map<int, int> &mapPinGpio);

    void buildButtonsFuncAssigned(const std::string &fileName, std::map<int, std::string> &mapButtonsFuncAssigned);

private:
    ConfigParser confParser;
    void setParentItem(std::vector<MenuItem *> &menu, MenuItem *processedMenuItem, uint32_t parentId);
    void setLeftItem(std::vector<MenuItem *> &menu, MenuItem *processedMenuItem, uint32_t leftId);
    void setRightItem(std::vector<MenuItem *> &menu, MenuItem *processedMenuItem, uint32_t rightId);
    std::string trim(const std::string &str, const std::string &whitespace =  \t);
};
}
