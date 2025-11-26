#include "Player.h"
#include "Screen.h"
#include "Keys.h"
#include <cctype> //  for tolower, isdigit

void Player::handleKeyPressed(char key_pressed) {
	size_t index = 0;
	for (char k : p_keys) {
		if (std::tolower(k) == std::tolower(key_pressed)) {
			position.setDirection((Keys)index);
			return;
		}
		++index;
	}
}

bool Player::addToInventory(char item)
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
		return false;
	}
	return true;
}

void Player::move() {//OFIR ADDITION - modified to prevent constant beeping
	point originalPos = position;

	position.draw();
	position.move();
	char next = map.getCharAt(position);
	if (next == objSigns::KEY) {
		//if(inventory != ' ')// maybe: check if inventory is full
		if (addToInventory(objSigns::KEY))
			map.setChar(position, ' ');
		else
			map.setChar(position, objSigns::KEY); //keep the key on the map
		
		
	}
	if(isdigit(next)) {
		if (hasItem(objSigns::KEY)) {//if its a door and Player has the key
			removeItem();
			if (map.getCurrentRoom() == 1)
				map.room1Challenge(next, position);
			return;
			//if(both players on door) load(next room)...s
		}
		else{ //if no key - the door is like a wall
			position = originalPos;
			position.setDirection(Keys::STAY);
			return;
		}
	}
	if (map.isWall(position)) {
		position = originalPos;
		position.setDirection(Keys::STAY);
		return;
	}
	if(position.getX() == originalPos.getX() && position.getY() == originalPos.getY()) {
		return;
	}
	originalPos.draw(' ');
	position.draw();
}