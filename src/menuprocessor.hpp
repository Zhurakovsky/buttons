#pragma once
#include "commontypes.hpp"
#include "menuitem.hpp"

#include <vector>
#include <cstdint>
#include <iterator>
#include <string>

namespace rpibuttons
{
class MenuProcessor
{
public:
    MenuProcessor();
    ~MenuProcessor();

    MenuItem *getActive();
    void setActive(MenuItem *newActive);
    MenuItem *getHead();
    MenuItem *getTail();

    void printChain();
    void printMenu();

    bool menuHasActive();
    void initActiveMenuItem();
    uint32_t getMenuSIze();

private:

};
}
