#pragma once

#include <string>

namespace rpibuttons
{

enum class RPiGPIOPin
 {
    RPI_V2_GPIO_P1_03 = 2,
    RPI_V2_GPIO_P1_05 = 3, 
    RPI_V2_GPIO_P1_07 = 4,
    RPI_V2_GPIO_P1_08 = 14,
    RPI_V2_GPIO_P1_10 = 15,
    RPI_V2_GPIO_P1_11 = 17,
    RPI_V2_GPIO_P1_12 = 18,
    RPI_V2_GPIO_P1_13 = 27,
    RPI_V2_GPIO_P1_15 = 22,
    RPI_V2_GPIO_P1_16 = 23,
    RPI_V2_GPIO_P1_18 = 24,
    RPI_V2_GPIO_P1_19 = 10,
    RPI_V2_GPIO_P1_21 = 9,
    RPI_V2_GPIO_P1_22 = 25,
    RPI_V2_GPIO_P1_23 = 11,
    RPI_V2_GPIO_P1_24 = 8,
    RPI_V2_GPIO_P1_26 = 7,
    RPI_V2_GPIO_P1_29 = 5,
    RPI_V2_GPIO_P1_31 = 6,
    RPI_V2_GPIO_P1_32 = 12,
    RPI_V2_GPIO_P1_33 = 13,
    RPI_V2_GPIO_P1_35 = 19,
    RPI_V2_GPIO_P1_36 = 16,
    RPI_V2_GPIO_P1_37 = 26,
    RPI_V2_GPIO_P1_38 = 20,
    RPI_V2_GPIO_P1_40 = 21
};

//RPiGPIOPin

//extern const RPiGPIOPin RPiGPIOPins[(int)RPiGPIOPin::Last+1];
const RPiGPIOPin RPiGPIOPins[] = 
{
	RPiGPIOPin::RPI_V2_GPIO_P1_03,
    RPiGPIOPin::RPI_V2_GPIO_P1_05, 
    RPiGPIOPin::RPI_V2_GPIO_P1_07,
    RPiGPIOPin::RPI_V2_GPIO_P1_08,
    RPiGPIOPin::RPI_V2_GPIO_P1_10,
    RPiGPIOPin::RPI_V2_GPIO_P1_11,
    RPiGPIOPin::RPI_V2_GPIO_P1_12,
    RPiGPIOPin::RPI_V2_GPIO_P1_13,
    RPiGPIOPin::RPI_V2_GPIO_P1_15,
    RPiGPIOPin::RPI_V2_GPIO_P1_16,
    RPiGPIOPin::RPI_V2_GPIO_P1_18,
    RPiGPIOPin::RPI_V2_GPIO_P1_19,
    RPiGPIOPin::RPI_V2_GPIO_P1_21,
    RPiGPIOPin::RPI_V2_GPIO_P1_22,
    RPiGPIOPin::RPI_V2_GPIO_P1_23,
    RPiGPIOPin::RPI_V2_GPIO_P1_24,
    RPiGPIOPin::RPI_V2_GPIO_P1_26,
    RPiGPIOPin::RPI_V2_GPIO_P1_29,
    RPiGPIOPin::RPI_V2_GPIO_P1_31,
    RPiGPIOPin::RPI_V2_GPIO_P1_32,
    RPiGPIOPin::RPI_V2_GPIO_P1_33,
    RPiGPIOPin::RPI_V2_GPIO_P1_35,
    RPiGPIOPin::RPI_V2_GPIO_P1_36,
    RPiGPIOPin::RPI_V2_GPIO_P1_37,
    RPiGPIOPin::RPI_V2_GPIO_P1_38,
    RPiGPIOPin::RPI_V2_GPIO_P1_40
};

std::ostream& operator<<(std::ostream& o, RPiGPIOPin x)
{
	return o << std::underlying_type<RPiGPIOPin>::type(x);
}

enum class MenuItemActionType
{
   DisplayText, //Display Text
   ClearScreen, //Clear whole or part of the screen
   DisplayClearGraphicsIcon, //Display / clear a graphics icon
   RunProgram//Run a program / script
};

struct ScreenRect
{
    int x;
    int y;
    int w;
    int h;
};

struct MenuItemActionProperties
{
    std::string textToShow;
    ScreenRect rectToClear;
    std::string pathToGraphicIcon;
    std::string pathToApplication;
};

}
