#pragma once
#include "point.h"

class Screen;//forward declaration to avoid circular dependency

class Switch {
    point position;
    bool isOn;
    bool wasToggled = false;
    Screen* map; 
    int targetDoorId = -1;

public:
    Switch(int x, int y, Screen* theMap, int targetDoorId = -1, bool initialState = false)
        : position(x, y, objSigns::SWITCH_OFF), map(theMap), isOn(initialState), targetDoorId(targetDoorId) {
    }

    void toggle();
    void update(bool isPressed);

    void setTargetDoorId(int doorId) {
        targetDoorId = doorId;
	}
    int getTargetDoorId() const {
        return targetDoorId;
	}
    bool isAt(const point& p) const {
        return position.getX() == p.getX() && position.getY() == p.getY();
    }
    bool getIsOn() const { return isOn; }
};