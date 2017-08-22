#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>      // std::ifstream

namespace rpibuttons
{

class ConfigParser
{
public:
    ConfigParser();
    ~ConfigParser();
    std::vector<std::string> getConfigStrings(const std::string &filename,
                                              const std::string &searchMask);


private:
    std::ifstream m_ifs;

};

}
