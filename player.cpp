#include "Player.h"
#include "Screen.h"
#include "Spring.h"
#include <cctype>
#include <windows.h>
#include <conio.h> //?

void Player::draw()
{
	{
		if (map.IsColor()) {
			char c = position.getChar();
			position.draw(c, color);
		}
		else { position.draw(WHITE); }
	}
}

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
		return false;
	}
	return true;
}

void Player::dispose()
{
	if (inventory[0] != ' ') {
		map.setChar(position, inventory[0]);
		if (inventory[0] == objSigns::BOMB) {
			map.addActiveBomb(position);
		}
		position.draw(map.IsColor() ? getColorForChar(position.getChar()) : WHITE);
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
	position.setChar(' ');
}

void Player::move() {
	if (!state) return; //if player movement is disabled

	applySpringDirectionIfNeeded();

	int stepsToTake = computeStepsToTake();
	for (int i = 0; i < stepsToTake; ++i) {
		// takeStep() will return true when movement should stop (blocked or state changed)
		if (takeStep()) break;
	}
	handleActiveSpring();
	finalizeMovement();
}



void Player::applySpringDirectionIfNeeded() {
	if (springCyclesLeft > 0) {
		position.setDirection(springDir.getDirectionEnum());
	}
}

int Player::computeStepsToTake() const {
	return (springCyclesLeft > 0) ? currentForce : 1;
}

bool Player::takeStep() {
	point originalPos = position;
	point nextCandidate = position;
	nextCandidate.move();
	char nextTile = map.getCharAt(nextCandidate);
	bool blocked = false;

	if (map.isWall(nextCandidate)) {
		blocked = true;
	}
	else { // not a wall
		int force = (springCyclesLeft > 0) ? currentForce : 1;
		blocked = handleSpecialObjects(nextTile, nextCandidate, force);
	}

	if (!state) return true;

	if (blocked) { // if we hit a wall or blocked object, we stop
		if (springCyclesLeft > 0) {
			springCyclesLeft = 0;
			currentForce = 1;
		}
		position = originalPos;
		return true; // stop further steps
	}
	else { // success: move to nextCandidate
		if (originalPos.getX() != nextCandidate.getX() || originalPos.getY() != nextCandidate.getY()) {
			if (map.getSpringAt(originalPos) == nullptr) {
				char underlying = map.getCharAt(originalPos);
				map.setChar(originalPos, underlying); // leaving no trail
			}
		}
		position = nextCandidate;
		position.draw(map.IsColor() ? getColorForChar(position.getChar()) : WHITE);
		return false; // can continue
	}
}

void Player::handleActiveSpring() {
	Spring* activeSpring = map.getSpringAt(position);
	// now we want to 'visualize' the spring if we are on one
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
}

void Player::finalizeMovement() {
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
	if (isdigit((unsigned char)nextTile))  // check if it's a door
		return atDoor(nextTile, nextPos);
		
	if (nextTile == objSigns::OBSTACLE) {
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
	finishedLevel = false;
}

bool Player::atDoor(unsigned char nextTile, point nextPos) {
	int doorId = nextTile - '0';
	if (map.ConnectionStatus(doorId)){ // connected to a switch
		if (map.SwitchState(doorId)) {return OpenDoorWithKey(doorId, nextPos);}//true = switch is on 
		else { //switch is off
			map.showMessage("The door is locked. Find the switch to open it.");
			return true;
		}
	}
	else { return OpenDoorWithKey(doorId, nextPos);}//not connected to a switch
	
}


bool Player::OpenDoorWithKey(int doorId, point nextPos) {
	if (map.isDoorOpen(doorId)) {
		clearFromScreen();
		finishedLevel = true;
		return false;
	}
	else {
		if (hasItem(objSigns::KEY)) {
			removeItem();
			map.showPlayerInfo(*this);
			if (map.isWinningDoor(doorId)) {
				map.openDoor(doorId);
				clearFromScreen();
				map.showMessage("Correct door! Unlocked.");
				finishedLevel = true;
				return false;
			}
			else {
				map.setChar(nextPos, 'X');
				map.showMessage("Wrong door! It's a dead end.");
				return true;
			}
		}
		else {
			map.showMessage("try look for a key to unlock this door");
			return true;
		}
	}
}