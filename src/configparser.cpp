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
    m_ifs.open(filename, std::ifstream::in);
    if ( !m_ifs.is_open())
    {
        std::cout << "Error opening " << filename.c_str() << std::endl;
    }
    std::string line;
    std::vector<std::string> result;
    while(std::getline(m_ifs, line))
    {
        if (line[0] == searchMask[0])
        {
            result.push_back(line);
        }
        else if (line[0] == '#')
        {
            continue;
        }
    }
    m_ifs.close();
    return result;
}


}
