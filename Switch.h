#pragma once
#include "Point.h"

class Screen;//forward declaration to avoid circular dependency

class Switch {
    Point position;
    bool isOn;
    bool wasToggled = false;
    Screen* board; 
    int targetDoorId = -1;

public:
    Switch(int x, int y, Screen* theMap, int targetDoorId = -1, bool initialState = false)
        : position(x, y, objSigns::SWITCH_OFF), board(theMap), isOn(initialState), targetDoorId(targetDoorId) {
    }

    void toggle();
    void update(bool isPressed);
    Point getPosition()const { return position; };
    void setTargetDoorId(int doorId) { targetDoorId = doorId; }
    int getTargetDoorId() const {return targetDoorId;}
    bool isAt(const Point& p) const {return position == p;}
    bool getIsOn() const { return isOn; }
};