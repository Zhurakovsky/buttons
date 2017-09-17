#pragma once

#include <iostream>
#include "commontypes.hpp"
#include <cinttypes>
#include <vector>

namespace rpibuttons
{

class MenuItem
{
public:
    MenuItem(int newId = 0);
    ~MenuItem();

    MenuItem* getNext();
    void setNext(MenuItem *newNext);

    MenuItem* getPrevious();
    void setPrevious(MenuItem *newPrevious);

    MenuItem* getParent();
    void setParent(MenuItem *newParent);

    MenuItem* getChild();
    void setChild(MenuItem *newChild);

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

    uint8_t *getBitmapData();
    void  setBitmapData(const uint8_t * bitmapData);
    uint16_t getBitmapW();
    uint16_t getBitmapH();

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
