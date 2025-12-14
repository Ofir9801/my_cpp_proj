#include "Switch.h"
#include "Screen.h"	

void Switch::toggle() {
	isOn = !isOn;
	char newSymbol = isOn ? objSigns::SWITCH_ON : objSigns::SWITCH_OFF;
	position.setChar(newSymbol);
	board->setChar(position, newSymbol);
	board->showMessage(isOn ? "Switch is ON" : "Switch is OFF");
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