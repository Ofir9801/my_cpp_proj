#include "Player.h"
#include "Screen.h"
#include "Keys.h"
#include "Spring.h"
#include <cctype>
#include <windows.h>

void Player::handleKeyPressed(char key_pressed) {
	if (springCyclesLeft > 0) return; //disable changing direction during spring flight
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
		Sleep(500);
		map.showMessage("                            ");
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
		Sleep(500);
		map.showMessage("                            ");
	}
}

void Player::clearFromScreen()
{
	state = false;
	position.setDirection(Keys::STAY);
	map.setChar(position, ' ');
	position.setChar(' ');
}

void Player::move() {
	if (!state) return; //if player movement is disabled
	if (springCyclesLeft > 0) {
		position.setDirection(springDir.getDirectionEnum());
	}
	if (map.getThroughDoor(this)) {
		map.setSuccessfulMove(true);
		clearFromScreen();
		return;
	}
	int stepsToTake = (springCyclesLeft > 0) ? currentForce : 1;
	for (int i = 0; i < stepsToTake; ++i) {
		point originalPos = position;
		point nextCandidate = position;
		nextCandidate.move();
		char nextTile = map.getCharAt(nextCandidate);
		bool blocked = false;
		if (map.isWall(nextCandidate)) {
			blocked = true;
		}
	
		else {//not a wall
			int force = (springCyclesLeft > 0) ? currentForce : 1;
			blocked = handleSpecialObjects(nextTile, nextCandidate, force);
		}

		// If some handler (e.g. room1Challenge) disabled the player and cleared the screen,
		// don't continue drawing / moving this player in this tick.
		if (!state) return;

		if (blocked) {//if we hit a wall or blocked object, we stop
			if (springCyclesLeft > 0) {
				springCyclesLeft = 0;
				currentForce = 1;
			}
			position = originalPos;
			break;
		}
		else {//success
			if (originalPos.getX() != nextCandidate.getX() || originalPos.getY() != nextCandidate.getY()) {
				if (map.getSpringAt(originalPos) == nullptr) {
					map.setChar(originalPos, ' '); // leaving no trail
				}
			}
			position = nextCandidate;
			position.draw();
		}
	}
	Spring* activeSpring = map.getSpringAt(position);
	//now we want to 'visualize' the spring if we are on one
	if (activeSpring != nullptr) {
		activeSpring->draw(position, true);
		point checkWall = position;
		checkWall.move();
		if (map.isWall(checkWall)) {
			int force = activeSpring->calculateForce(position);
			springCyclesLeft = force * force;
			currentForce = force;
			springDir = position;
			springDir.setDirection(activeSpring->getDirection());
			position.setDirection(activeSpring->getDirection());
		}
	}
	if (springCyclesLeft > 0) {
		springCyclesLeft--;
		if (springCyclesLeft == 0) currentForce = 1;
	}
}

bool Player::handleSpecialObjects(char nextTile, point nextPos, int force) {//function to handle special objects
	if (nextTile == objSigns::KEY) {
		if (addToInventory(objSigns::KEY)) {
			// clear the key from the tile we are moving onto (nextPos), not the player's old position
			map.setChar(nextPos, ' ');
			return false;
		}
		return true; //if inventory is full its blocked
	}

	if(isdigit((unsigned char)nextTile)) {//door
		if (hasItem(objSigns::KEY)) {
			removeItem();
			if (map.getCurrentRoom() == roomIndex::ROOM1) {
				// pass the tile we are moving onto (nextPos) so the screen can clear that cell
				map.room1Challenge(nextTile, nextPos, this);
			}
			return false;
		}
		return true;
	}
	if (nextTile == '*') {//obstacle
		Keys pushDir = (springCyclesLeft > 0) ? springDir.getDirectionEnum() : position.getDirectionEnum();

		if (map.tryPushObstacle(nextPos, pushDir, force)) {
			return false;
		}
		return true;
	}
	return false;
}
void Player::reset(point newPosition) {
	position = newPosition;
	state = true;
	position.setDirection(Keys::STAY);
	springCyclesLeft = 0;
	currentForce = 1;
}
Keys Player::getOppositeDirection(Keys dir) {
	switch (dir) {
	case Keys::UP: return Keys::DOWN;
	case Keys::DOWN: return Keys::UP;
	case Keys::LEFT: return Keys::RIGHT;
	case Keys::RIGHT: return Keys::LEFT;
	default: return Keys::STAY;
	}
}