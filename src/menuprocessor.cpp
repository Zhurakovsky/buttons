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

MenuItem* MenuProcessor::getActive()
{
    return m_activeItem;
}

void MenuProcessor::setActive(MenuItem *newActive)
{
    m_activeItem = newActive;
}

MenuItem* MenuProcessor::getHead(MenuItem *item)
{
    MenuItem *headItem = item;
    while (headItem->hasPrevious())
    {
        headItem = headItem->getPrevious();
    }
    return headItem;
}

MenuItem *MenuProcessor::getTail(MenuItem *item)
{
    MenuItem *tailItem = item;
    while (tailItem->hasNext())
    {
        tailItem = tailItem->getNext();
    }
    return tailItem;
}

void MenuProcessor::printChain()
{
    MenuItem *item = getHead(m_activeItem);
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
    MenuItem *activeItem = getActive();
    printChain(activeItem);
}

}
