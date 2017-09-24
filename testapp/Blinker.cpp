#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include "Gpioclass.hpp"

using namespace std;

void usage(const char* s)
{
    std::cout << "Usage: sudo " << s << " <gpioport> <number of blinks>" << std::endl;
    exit (EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    std::string pinNumber;
    uint32_t blinkCount;

    if (argc < 3) // We expect 3 arguments: the program name, the source path and the destination path
    {
        usage(argv[0]);
        //return 1;
        pinNumber = "4";
        blinkCount = 10;
    }
    else
    {
        pinNumber = argv[1];
        blinkCount = (uint32_t)atoi(argv[2]);
    }

    std::string inputstate;
    GPIOClass *gpio4 = new GPIOClass(pinNumber);
    gpio4->export_gpio();

    std::cout << " GPIO pins exported" << std::endl;
    gpio4->setdir_gpio("out"); // gpio4 set to output

    std::cout << "Set GPIO pin direction" << std::endl;

    while (blinkCount--)
    {
        usleep(500000);
        gpio4->getval_gpio(inputstate);

        std::cout << "Current input pin state is " << inputstate.c_str() << std::endl;
        if(inputstate == "0") // if input pin is at state "0" i.e. button pressed
        {
            gpio4->setval_gpio("1"); // turn LED ON
        }
        else if (inputstate == "1")
        {
            gpio4->setval_gpio("0");
        }
    }

    std::cout << "Hello from blinker" << std::endl;
    return 0;
}
