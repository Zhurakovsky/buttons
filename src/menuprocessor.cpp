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

MenuItem *MenuProcessor::getHead(const MenuItem *item)
{
    MenuItem *headItem = item;
    while (item->getPrevious() != nullptr)
    {
        headItem = item->getPrevious();
    }
    return headItem;
}

MenuItem *MenuProcessor::getTail(const MenuItem *item)
{
    MenuItem *tailItem = item;
    while (item->getNext() != nullptr)
    {
        tailItem = item->getNext();
    }
    return tailItem;
}

void MenuProcessor::printChain(const MenuItem *activeItem)
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

void MenuProcessor::printMenu()
{
    MenuItem *activeItem = getActive(m_menu);
    printChain(activeItem);
}

}
