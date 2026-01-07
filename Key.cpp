#include "key.h"
#include <iostream>
#include <fstream>

void Key::save(std::ofstream& file) const {
	position.save(file);
	file << targetDoorId << " ";
	file << static_cast<int>(playerUse) << "\n";
}

void Key::load(std::ifstream& file) {
	int tempCh;
	position.load(file);
	file.ignore();
	file >> targetDoorId;
	file >> tempCh;
	playerUse = static_cast<char>(tempCh);
}


