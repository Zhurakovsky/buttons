#include "menuitem.hpp"
namespace rpibuttons
{
MenuItem::MenuItem(int newId)
    : next(nullptr),
      previous(nullptr),
      parent(nullptr),
      child(nullptr),
      m_isActive(false),
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

MenuItem* MenuItem::getPrevious()
{
    return previous;
}

void MenuItem::setPrevious(MenuItem *newPrevious)
{
    previous = newPrevious;
}

MenuItem* MenuItem::getParent()
{
    return parent;
}

void MenuItem::setParent(MenuItem *newParent)
{
    parent = newParent;
}

MenuItem* MenuItem::getChild()
{
    return child;
}

void MenuItem::setChild(MenuItem *newChild)
{
    child = newChild;
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

}
