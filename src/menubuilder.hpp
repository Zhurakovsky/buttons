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
};
}
