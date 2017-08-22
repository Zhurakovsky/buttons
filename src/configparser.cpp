#include "configparser.hpp"

namespace rpibuttons
{

ConfigParser::ConfigParser()
{

}

ConfigParser::~ConfigParser()
{

}

std::vector<std::string> ConfigParser::getConfigStrings(const std::string &filename, const std::__cxx11::string &searchMask)
{
    m_ifs.open(filename, std::ifstream::in);
    std::string line;
    std::vector<std::string> result;
    while(std::getline(m_ifs, line))
    {
        std::cout << "line:" << line << std::endl;
        if (line[0] == "M")
        {
            result.push_back(line);
        }
        // TODO: assign item_name based on line (or if the entire line is
        // the item name, replace line with item_name in the code above)
    }
    return result;
}


}
