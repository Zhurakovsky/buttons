#include "menuprocessor.hpp"
#include <iostream>


namespace rpibuttons
{

MenuProcessor::MenuProcessor(const std::vector<MenuItem*> &menu)
    : m_menu(menu)
{

}

MenuProcessor::~MenuProcessor()
{

}

MenuItem *MenuProcessor::getActive(const std::vector<MenuItem *> &menu) const
{
    MenuItem *activeMenuItem = nullptr;
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        MenuItem *tmpMenuItem = *it;
        if (tmpMenuItem->isActive())
        {
            activeMenuItem = tmpMenuItem;
            break;
        }
    }
    return activeMenuItem;
}

MenuItem* MenuProcessor::getHead(MenuItem *item) const
{
    MenuItem *headItem = item;
    while (item->getPrevious() != nullptr)
    {
        headItem = item->getPrevious();
    }
    return headItem;
}

MenuItem *MenuProcessor::getTail(MenuItem *item) const
{
    MenuItem *tailItem = item;
    while (item->getNext() != nullptr)
    {
        tailItem = item->getNext();
    }
    return tailItem;
}

void MenuProcessor::printChain(MenuItem *activeItem) const
{
    MenuItem *item = getHead(activeItem);
    while (item != nullptr)
    {
        std::string itemCapture = item->itemName();
        std::cout << itemCapture.c_str();
        if (item->isActive())
        {
            std::cout << " <<<";
        }
        std::cout << std::endl;
        item = item->getNext();
    }
}

void MenuProcessor::printMenu() const
{
    MenuItem *activeItem = getActive(m_menu);
    printChain(activeItem);
}

}
