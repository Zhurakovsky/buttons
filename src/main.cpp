#include <iostream>
#include "buttonListener.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"

#include <vector>
#include <iterator>

#include <bcm2835.h>

using namespace std;
using namespace rpibuttons;

int main()
{
    ButtonListener bl;
    std::vector<MenuItem*> menu;
    MenuItem *mi0 = new MenuItem();
    mi0->setActive(true);
    mi0->setItemName("Item 0");
    menu.push_back(mi0);

    MenuItem *mi1 = new MenuItem(1);
    mi0->setNext(mi1);
    //mi1->setActive(true);
    mi1->setItemName("Item 1");
    mi1->setPrevious(mi0);
    menu.push_back(mi1);

    MenuItem *mi2 = new MenuItem(2);
    mi1->setNext(mi2);
    //mi1->setActive(true);
    mi2->setItemName("Item 2");
    mi2->setPrevious(mi1);
    menu.push_back(mi2);

    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *tmpItem = *it;
        std::cout << tmpItem->itemName().c_str() << std::endl;
    }

    bl.run();
    std::cout << "Hello, Buttons" << std::endl;
    return 0;
}
