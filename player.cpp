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

void player::move() {//OFIR ADDITION - modified to prevent constant beeping
	point originalPos = position;
	position.draw();
	position.move();

	if (map.isWall(position)) {
		position = originalPos;
		position.setDirection(keys::STAY);//OFIR ADDITION if hit wall, stay in place
		return;
	}
	if(position.getX() == originalPos.getX() && position.getY() == originalPos.getY()) {
		return; //OFIR ADDITION - no movement, so no need to redraw
	}
	originalPos.draw(' '); //OFIR ADDITION - erase previous position
	position.draw();
}