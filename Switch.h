#pragma once
#include "point.h"
#include "Screen.h"

class Switch {
    point position;
    bool isOn;
    bool wasToggled = false;
    Screen& map; 

public:
    Switch(int x, int y, Screen& theMap, bool initialState = false)
        : position(x, y, '/'), map(theMap), isOn(initialState) {
    }

    void toggle();
    void update(bool isPressed);

    bool isAt(const point& p) const {
        return position.getX() == p.getX() && position.getY() == p.getY();
    }
};