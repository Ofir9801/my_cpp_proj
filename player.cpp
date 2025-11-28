#include "Player.h"
#include "Screen.h"
#include "Keys.h"
#include <cctype>

void Player::handleKeyPressed(char key_pressed) {
	size_t index = 0;
	for (char k : p_keys) {
		if (std::tolower((unsigned char)k) == std::tolower(key_pressed)) {
			if(index != NUM_KEYS - 1){
				position.setDirection((Keys)index);
				return;
			}
			else {
				dispose();
				return;
			}
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

void Player::dispose()
{
	if (inventory[0] != ' ') {
		map.setChar(position, inventory[0]);
		position.draw();
		inventory[0] = ' ';
	}
	else {
		map.showMessage("No item to dispose.");
	}
}

void Player::clearFromScreen()
{
	state = false;
	position.setDirection(Keys::STAY);
	map.setChar(position, ' ');
}

void Player::move() {//OFIR ADDITION - modified to prevent constant beeping
	if (!state) return; //if player movement is disabled
	point originalPos = position;

	position.draw();
	position.move();
	char next = map.getCharAt(position);
	if (next == objSigns::KEY) {
		if (addToInventory(objSigns::KEY))
			map.setChar(position, ' ');
		else
			map.setChar(position, objSigns::KEY); //keep the key on the map
	}

	if (isdigit((unsigned char)next)) {
		if (hasItem(objSigns::KEY)) {//if its a door and Player has the key
			removeItem();
			if (map.getCurrentRoom() == roomIndex::ROOM1) {
				map.room1Challenge(next, position, this);
			}
		}
		//if(both players on door) load(next room)...
		else { //if no key - the door is like a wall
			position = originalPos;
			position.setDirection(Keys::STAY);
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
	originalPos.draw(map.getCharAt(originalPos));
	position.draw();
}
void Player::reset(point newPosition) {
	position = newPosition;
	state = true;
	position.setDirection(Keys::STAY);
}