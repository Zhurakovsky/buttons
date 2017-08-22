#pragma once

#include <iostream>
#include <string>
#include "configparser.hpp"


namespace rpibuttons
{
class MenuBuilder
{
public:
    void buildMenu(const std::string &fileName);

private:
    ConfigParser confParser;
};
}
