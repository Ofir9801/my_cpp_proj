#pragma once
#include "point.h"
class Switch
{
	point position;
	bool isOn;
public:
	Switch(int x, int y, bool initialState = false) : position(x, y, '/'), isOn(initialState) {}
	void toggle() {
		isOn = !isOn;
		position.setChar(isOn ? '\\' : '/');
		position.draw(); 
	}
	bool getState() const {
		return isOn;
	}
};


