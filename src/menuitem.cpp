#include "menuitem.hpp"
#include <iostream>
namespace rpibuttons
{
MenuItem::MenuItem(int newId)
    : m_isActive(false),
      actionType(MenuItemActionType::MenuDropdown),
      m_itemName(""),
      itemId(newId)
{
    next = NULL;
    previous = NULL;
    parent = NULL;
    child = NULL;
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
    if (!next)
    {
        return false;
    }

    return true;
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
    if (!previous)
    {
        return false;
    }
    return true;
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
    if (!previous)
    {
        return false;
    }
    return true;
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
    if (!child)
    {
        return false;
    }
    return true;
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
