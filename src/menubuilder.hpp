#pragma once

#include <iostream>
#include <string>
#include "configparser.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"


namespace rpibuttons
{
class MenuBuilder
{
public:
    void buildMenu(const std::string &fileName, std::vector<MenuItem *> &menu);

private:
    ConfigParser confParser;
};
}
