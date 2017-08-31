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
    MenuProcessor(const std::vector<MenuItem*> &menu);
    ~MenuProcessor();

    MenuItem *getActive();
    void setActive(MenuItem *newActive);
    MenuItem *getHead(MenuItem *item);
    MenuItem *getTail(MenuItem *item);

    void printChain();
    void printMenu();

    bool menuHasActive();
    void initActiveMenuItem();
    uint32_t getMenuSIze();

private:
    std::vector<MenuItem*> m_menu;
    MenuItem *m_activeItem;

};
}
