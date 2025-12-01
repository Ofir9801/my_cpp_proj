#pragma once
#include "point.h"
#include "Screen.h"
#include "objSigns.h"

class Switch {
    point position;
    bool isOn;
    bool wasToggled = false;
    Screen& map; 
    int targetDoorId = -1;

public:
    Switch(int x, int y, Screen& theMap, bool initialState = false)
        : position(x, y, objSigns::SWITCH_OFF), map(theMap), isOn(initialState) {
    }

    void toggle();
    bool update(bool isPressed);

    void setTargetDoorId(int doorId) {
        targetDoorId = doorId;
	}
    int getTargetDoorId() const {
        return targetDoorId;
	}
    bool isAt(const point& p) const {
        return position.getX() == p.getX() && position.getY() == p.getY();
    }
    bool isAt(const point& p) const { return position.getX() == p.getX() && position.getY() == p.getY(); }
    bool getIsOn() const { return isOn; }
};