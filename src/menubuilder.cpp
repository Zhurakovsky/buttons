#include "menubuilder.hpp"

namespace rpibuttons
{

void MenuBuilder::buildMenu(const std::string &fileName)
{
    const std::string searchPattern = "M";
    std::vector<std::string> menuList;
    menuList = confParser.getConfigStrings(fileName, searchPattern);

    for(auto it = menuList.begin(); it != menuList.end(); ++it )
    {
        std::cout << *it <<std::endl;
    }
}


}
