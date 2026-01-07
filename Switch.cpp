#include "Switch.h"
#include "Screen.h"	
#include <iostream>
#include <fstream>
void Switch::toggle() {
	isOn = !isOn;
	objSigns newSymbol = isOn ? objSigns::SWITCH_ON : objSigns::SWITCH_OFF;
	position.setChar(newSymbol);
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

void Switch::save(std::ofstream& file) const {
	position.save(file);
	file << (isOn ? 1 : 0) << " ";
	file << (wasToggled ? 1 : 0) << " ";
	file << targetDoorId << "\n";
}
void Switch::load(std::ifstream& file) {
	position.load(file);
	bool flag;
	file >> flag;
	isOn = (flag == 1);
	file >> flag;
	wasToggled = (flag == 1);
	file >> targetDoorId;
}