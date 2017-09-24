#ifndef GPIO_CLASS_HPP
#define GPIO_CLASS_HPP

#include <string>

using namespace std;

class GPIOClass
{
public:

    GPIOClass();
    GPIOClass(const std::string & gpionum);
    ~GPIOClass();

    int export_gpio();
    int unexport_gpio();
    int setdir_gpio(std::string dir);
    int setval_gpio(std::string val);
    int getval_gpio(std::string& val);

    std::get_gpionum();

private:
    std::string m_gpionum;
};


#endif
