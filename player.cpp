#include "player.h"
#include "screen.h"
#include "keys.h"

void player::handleKeyPressed(char key_pressed) {
	size_t index = 0;
	for (char k : p_keys) {
		if (std::tolower(k) == std::tolower(key_pressed)) {
			position.setDirection((keys)index);
			return;
		}
		++index;
	}
}

void player::addToInventory(char item)
{
	bool added = false;
	for (int i = 0; i < INVENTORY_SIZE && !added ; ++i) {
		if (inventory[i] == ' ') {
			inventory[i] = item; //add item to inventory
			added = true;
		}
	}
	if (!added) {
		map.showMessage("Inventory full!");
	}
}

void player::move() {
	position.draw(' ');
	point body0orig = position;
	position.move();

	if (map.isWall(position)) {
		position = body0orig;
		position.setDirection(keys::STAY);//OFIR ADDITION if hit wall, stay in place
	}
	position.draw();
}