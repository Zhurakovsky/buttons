#include "menuprocessor.hpp"
#include <iostream>


namespace rpibuttons
{

MenuProcessor::MenuProcessor()
{

}

MenuProcessor::~MenuProcessor()
{

}

MenuItem* MenuProcessor::getActive()
{

}

void MenuProcessor::setActive(MenuItem *newActive)
{
}

MenuItem* MenuProcessor::getHead()
{
}

MenuItem *MenuProcessor::getTail()
{
    MenuItem *tailItem = item;
    while ((tailItem->getNext() != NULL))
    {
        tailItem = tailItem->getNext();
    }
    return tailItem;
}

void MenuProcessor::printChain()
{
    MenuItem *item = getHead(m_activeItem);
    while (item != NULL)
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
    //MenuItem *activeItem = getActive();
    std::cout << "Live 5" << std::endl;
    printChain();
}

bool MenuProcessor::menuHasActive()
{
    bool res = false;
    for (auto it = m_menu.begin(); it != m_menu.end(); ++it)
    {
        MenuItem *tmpItem = *it;
        if (tmpItem->isActive())
        {
            res = true;
            break;
        }
    }
    return res;
}

void MenuProcessor::initActiveMenuItem()
{
    if (getMenuSIze() > 0)
    {
        if (!menuHasActive())
        {
            auto it = m_menu.begin();
            if (it != m_menu.end())
            {
                MenuItem *tmpItem = *it;
                tmpItem->setActive(true);
                m_activeItem = *it;
            }
        }
        else
        {
            m_activeItem = getActive();
        }
    }
}

uint32_t MenuProcessor::getMenuSIze()
{
    return (uint32_t)m_menu.size();
}

}
