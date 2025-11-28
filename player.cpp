#include "Player.h"
#include "Screen.h"
#include "Keys.h"
#include <cctype>
#include <windows.h>

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

void Player::move() {
	if (!state) return; //if player movement is disabled
	point originalPos = position;
	if (springCyclesLeft > 0) {
		handleSpringFlight();//helper function for spring flight
		return;
	}
	position.move();
	int force = 1;
	char nextTile = map.getCharAt(position);
	bool blocked = handleSpecialObjects(nextTile, originalPos, force);//helper function for special objects

	if (blocked || map.isWall(position)) {
		position = originalPos;
		position.setDirection(Keys::STAY);
		return;
		}
	if (position.getX() == originalPos.getX() && position.getY() == originalPos.getY()) {
		return;//to prevent issues with beeping 
	}
	originalPos.draw(map.getCharAt(originalPos));
	position.draw();
}
void Player::handleSpringFlight() {//function to handle the sping effect
	if (springCyclesLeft <= 0) return;
	for (int i = 0; i < currentForce; ++i) {
		point prevPos = position;
		point nextPos = position;
		nextPos.setDirection(springDir.getDirectionEnum());
		nextPos.move();

		if (map.isWall(nextPos)) {
			springCyclesLeft = 0;
			currentForce = 1;
			break;
		}
		prevPos.draw(map.getCharAt(prevPos));
		position = nextPos;
		position.draw();
		Sleep(50);
	}

	springCyclesLeft--;
	if (springCyclesLeft == 0) currentForce = 1;
}
bool Player::handleSpecialObjects(char nextTile, point originalPos, int force) {//function to handle special objects
	if (nextTile == objSigns::KEY) {
		if (addToInventory(objSigns::KEY)) {
			map.setChar(position, ' ');
			return false;
		}
		return true; //if inventory is full its blocked
	}

	if (nextTile == '*') {//obstacle
		Keys pushDir = (springCyclesLeft > 0) ? springDir.getDirectionEnum() : position.getDirectionEnum();

		if (map.tryPushObstacle(position, pushDir, force)) {
			return false;
		}
		return true;
	}
	if (isdigit((unsigned char)nextTile)) {//door
		if (hasItem(objSigns::KEY)) {
			removeItem();
			if (map.getCurrentRoom() == roomIndex::ROOM1) {
				map.room1Challenge(nextTile, position, this);
			}
			return false;
		}
		return true;
	}

	if (nextTile == '#' && springCyclesLeft == 0) {//new spring
		Keys dir = position.getDirectionEnum();
		point nextPos = position;
		int springSize = 0;
		while(map.getCharAt(nextPos) == '#') {
			springSize++;
			nextPos.setDirection(dir);
			nextPos.move();
			if (map.isWall(nextPos)) break;
		}
		currentForce = springSize;
		springCyclesLeft = springSize * springSize;
		springDir = position;
		springDir.setDirection(dir);
		return false;
	}

	return false;
}

void Player::reset(point newPosition) {
	position = newPosition;
	state = true;
	position.setDirection(Keys::STAY);
}
int Player::countSpringChars(point startPos, Keys dir) {
	int count = 0;
	point tempPos = startPos;
	tempPos.setDirection(dir);
	tempPos.move();
	while (map.getCharAt(tempPos) == '#') {
		count++;
		tempPos.setDirection(dir);
		tempPos.move();
		if (map.isWall(tempPos)) break;
	}
	return count;
}