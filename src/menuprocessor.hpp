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

    MenuItem *getActive(const std::vector<MenuItem*> &menu) const;
    MenuItem *getHead(const MenuItem *item);
    MenuItem *getTail(const MenuItem *item);

    void printChain(const MenuItem *activeItem);
    void printMenu();

private:
    std::vector<MenuItem*> m_menu;

};
}
