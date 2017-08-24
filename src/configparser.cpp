#include "configparser.hpp"
//#include <cstring>

namespace rpibuttons
{

ConfigParser::ConfigParser()
{

}

ConfigParser::~ConfigParser()
{

}

std::vector<std::string> ConfigParser::getConfigStrings(const std::string &filename, const std::string &searchMask)
{
    if (m_ifs.open(filename, std::ifstream::in) < 0)
    {
        std::cout << "Error opening " << filename.c_str() << std::endl;
    }
    std::string line;
    std::vector<std::string> result;
    while(std::getline(m_ifs, line))
    {
//        std::cout << "line:" << line << std::endl;
//        const char* mPattern = "M";
        if (line[0] == 'M')
        {
            result.push_back(line);
        }
        else if (line[0] == '#')
        {
            continue;
        }
        // TODO: assign item_name based on line (or if the entire line is
        // the item name, replace line with item_name in the code above)
    }
    return result;
}


}
