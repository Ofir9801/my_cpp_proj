#include "Switch.h"


void Switch::toggle() {
	isOn = !isOn;
	char newSymbol = isOn ? '\\' : '/';
	position.setChar(newSymbol);
	map.setChar(position, newSymbol);
}

void Switch::update(bool isPressed) {
	if (isPressed) {
		if (!wasToggled) {
			toggle();
			wasToggled = true;
		}
	}
	else
		wasToggled = false;
	}