#pragma once
#include "Door.h"
#include <iostream>
#include <fstream>

void Door::save(std::ofstream& file) const {
	position.save(file);
	file <<static_cast<int>(id) << " ";
	file << (isOpen ? 1 : 0) << " ";
	file << (connection ? 1 : 0) << "\n";
}

void Door::load(std::ifstream& file) {
	position.load(file);
	file.ignore();
	int tempVal;
	file >> tempVal;
	id = static_cast<char>(tempVal);
	bool flag;
	file >> flag;
	isOpen = (flag == 1);
	file >> flag;
	connection = (flag == 1);
}