#pragma once

#include <iostream>
#include "commontypes.hpp"
#include <cinttypes>

namespace rpibuttons
{

class MenuItem
{
public:
    MenuItem(int newId = 0);
    ~MenuItem();

    MenuItem* getNext();
    void setNext(MenuItem *newNext);
    static bool hasNext();

    MenuItem* getPrevious();
    void setPrevious(MenuItem *newPrevious);
    static bool hasPrevious();

    MenuItem* getParent();
    void setParent(MenuItem *newParent);
    static bool hasParent();

    MenuItem* getChild();
    void setChild(MenuItem *newChild);
    static bool hasChild();

    bool doAction();

    MenuItemActionType getActionType();
    void setActionType(const MenuItemActionType& act);

    MenuItemActionProperties getMenuItemProperties();
    void setMenuItemProperties(const MenuItemActionProperties &newProp);

    bool isActive();
    void setActive(bool status);

    std::string itemName();
    void setItemName(const std::string& newName);

    uint32_t getItemId() const;

private:
    MenuItem* next;
    MenuItem* previous;
    MenuItem* parent;
    MenuItem* child;

    MenuItemActionProperties prop;
    bool m_isActive;
    MenuItemActionType actionType;
    std::string m_itemName;
    uint32_t itemId;
};

}
