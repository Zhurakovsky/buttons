#include "Gpioclass.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>

GPIOClass::GPIOClass()
{
    m_gpionum = "4";
}

GPIOClass::GPIOClass(const string &gpionum)
{
    m_gpionum = gpionum;
}

GPIOClass::~GPIOClass()
{

}

int GPIOClass::export_gpio()
{
    std::string export_str = "/sys/class/gpio/export";
    std::ofstream exportgpio(export_str.c_str());
    if (exportgpio < 0)
    {
        std::cout << " OPERATION FAILED: Unable to export GPIO" << m_gpionum << endl;
        return -1;
    }

    exportgpio << m_gpionum;
    exportgpio.close();

    return 0;
}

int GPIOClass::unexport_gpio()
{
    std::string unexport_str = "/sys/class/gpio/unexport";
    ofstream unexporegpio(unexport_str.c_str());
    if (unexporegpio < 0)
    {
        std::cout << " OPERATION FAILED: Unable to unexport GPIO" << m_gpionum << endl;
        return -1;
    }

    unexporegpio << m_gpionum;
    unexporegpio.close();

    return 0;
}

int GPIOClass::setdir_gpio(std::string dir)
{
    std::string setdir_str ="/sys/class/gpio/gpio" + m_gpionum.c_str() + "/direction";
    std::cout << setdir_str.c_str() << std::endl;
    ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio

    if (setdirgpio < 0)
    {
        cout << " OPERATION FAILED: Unable to set direction of GPIO"<< m_gpionum << endl;
        return -1;
    }

    setdirgpio << dir.c_str(); //write direction to direction file
    setdirgpio.close(); // close direction file
    return 0;
}

int GPIOClass::setval_gpio(string val)
{
    string setval_str = "/sys/class/gpio/gpio" + m_gpionum.c_str() + "/value";
    std::cout << setval_str.c_str() << std::endl;
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
    if (setvalgpio < 0)
    {
        cout << " OPERATION FAILED: Unable to set the value of GPIO" << m_gpionum << endl;
        return -1;
    }

    setvalgpio << val.c_str() ;//write value to value file
    setvalgpio.close();// close value file
    return 0;
}

int GPIOClass::getval_gpio(string &val)
{
    string getval_str = "/sys/class/gpio/gpio" + m_gpionum + "/value";
    ifstream getvalgpio(getval_str.c_str());// open value file for gpio

    if (getvalgpio < 0)
    {
        cout << " OPERATION FAILED: Unable to get value of GPIO"<< m_gpionum << endl;
        return -1;
    }

    getvalgpio >> val;  //read gpio value

    if(val != "0")
    {
        val = "1";
    }
    else
    {
        val = "0";
    }

    getvalgpio.close(); //close the value file
    return 0;
}

string GPIOClass::get_gpionum()
{
    return m_gpionum;
}
