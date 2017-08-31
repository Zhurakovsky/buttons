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
    MenuItem *getHead(MenuItem *item) const;
    MenuItem *getTail(MenuItem *item) const;

    void printChain(MenuItem *activeItem) const;
    void printMenu() const;

private:
    std::vector<MenuItem*> m_menu;

};
}
