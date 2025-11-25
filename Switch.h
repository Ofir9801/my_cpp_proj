#pragma once
#include "point.h"
#include "Screen.h"

class Switch {
    point position;
    bool isOn;
    Screen& map; 

public:
    Switch(int x, int y, Screen& theMap, bool initialState = false)
        : position(x, y, '/'), map(theMap), isOn(initialState) {
    }

    void toggle() {
        isOn = !isOn;
        char newSymbol = isOn ? '\\' : '/';
        position.setChar(newSymbol);
        map.setChar(position, newSymbol);
    }

    bool isAt(const point& p) const {
        return position.getX() == p.getX() && position.getY() == p.getY();
    }
};