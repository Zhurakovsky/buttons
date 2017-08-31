#include "menuitem.hpp"
#include <iostream>
namespace rpibuttons
{
MenuItem::MenuItem(int newId)
    : next(nullptr),
      previous(nullptr),
      parent(nullptr),
      child(nullptr),
      m_isActive(false),
      actionType(MenuItemActionType::MenuDropdown),
      m_itemName(""),
      itemId(newId)
{

}

MenuItem::~MenuItem()
{

}

MenuItem* MenuItem::getNext()
{
    return next;
}

void MenuItem::setNext(MenuItem *newNext)
{
    next = newNext;
}

bool MenuItem::hasNext()
{
    return !(next == nullptr);
}

MenuItem* MenuItem::getPrevious()
{
    return previous;
}

void MenuItem::setPrevious(MenuItem *newPrevious)
{
    previous = newPrevious;
}

bool MenuItem::hasPrevious()
{
    std::cout << "Live 7" << std::endl;
    if (previous == nullptr)
    {
        return false;
    }
    else
    {
        return true;
    }
}

MenuItem* MenuItem::getParent()
{
    return parent;
}

void MenuItem::setParent(MenuItem *newParent)
{
    parent = newParent;
}

bool MenuItem::hasParent()
{
    return !(parent == nullptr);
}

MenuItem* MenuItem::getChild()
{
    return child;
}

void MenuItem::setChild(MenuItem *newChild)
{
    child = newChild;
}

bool MenuItem::hasChild()
{
    return !(child == nullptr);
}

bool MenuItem::doAction()
{
    return true;
}

MenuItemActionType MenuItem::getActionType()
{
    return actionType;
}

void MenuItem::setActionType(const MenuItemActionType &act)
{
    actionType = act;
}

MenuItemActionProperties MenuItem::getMenuItemProperties()
{
    return prop;
}

void MenuItem::setMenuItemProperties(const MenuItemActionProperties &newProp)
{
    prop = newProp;
}

bool MenuItem::isActive()
{
    return m_isActive;
}

void MenuItem::setActive(bool status)
{
    m_isActive = status;
}

std::string MenuItem::itemName()
{
    return m_itemName;
}

void MenuItem::setItemName(const std::string &newName)
{
    m_itemName = newName;
}

uint32_t MenuItem::getItemId() const
{
    return itemId;
}

}
