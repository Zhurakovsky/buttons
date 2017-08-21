#pragma once

#include <iostream>
#include "commontypes.hpp"

namespace rpibuttons
{

class MenuItem
{
public:
    MenuItem();
    ~MenuItem();

    MenuItem* getNext();
    void setNext(const MenuItem *newNext);

    MenuItem* getPrevious();
    void setPrevious(const MenuItem *newPrevious);

    MenuItem* getParent();
    void setParent(const MenuItem *newParent);

    MenuItem* getChild();
    void setChild(const MenuItem *newChild);

    bool doAction();

    MenuItemActionType getActionType();
    bool setActionType(const MenuItemActionType& act);

    MenuItemActionProperties getMenuItemProperties();
    bool setMenuItemProperties(const MenuItemActionProperties &newProp);

    bool isActive();
    void setActive(bool status);

    std::string itemName();
    void setItemName(const std::string& newName);

private:
    MenuItem* next;
    MenuItem* previous;
    MenuItem* parent;
    MenuItem* child;
    MenuItemActionType actionType;
    MenuItemActionProperties prop;
    bool m_isActive;
    std::string m_itemName;

};

}
