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
