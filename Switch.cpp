#include "Switch.h"


void Switch::toggle() {
	isOn = !isOn;
	char newSymbol = isOn ? objSigns::SWITCH_ON : objSigns::SWITCH_OFF;
	position.setChar(newSymbol);
	map.setChar(position, newSymbol);
	map.showMessage(isOn ? "Switch is ON" : "Switch is OFF");
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