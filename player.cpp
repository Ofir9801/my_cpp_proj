#include "Player.h"
#include "Screen.h"
#include "Spring.h"
#include <cctype>
#include <windows.h>

void Player::draw()
{
	{
		if (board.IsColor()) {
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
				position.setDirection((Keyboard_bind)index);
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

bool Player::addToInventory(char item, Point pos)
{
	bool added = false;
	for (int i = 0; i < INVENTORY_SIZE && !added ; ++i) {
		if (inventory[i] == ' ') {
			inventory[i] = item; //add item to inventory
			added = true;
			if (item == objSigns::KEY)
				board.addKeyToInventory(pos, this->getChar());
		}
	}
	if (!added) {
		board.showMessage("Inventory full!");
		return false;
	}
	return true;
}

void Player::removeItem()
{
	if (inventory[0] == objSigns::KEY){board.RemoveKeyFromInventory(this->getChar(), position);}
	inventory[0] = ' '; 
}

void Player::dispose()
{
	if (inventory[0] != ' ') { //fix this function
		char c = inventory[0];
		board.setChar(position, c);
		if (c == objSigns::KEY) {board.RemoveKeyFromInventory(this->getChar(), position);}
		else if (c == objSigns::BOMB) {board.addActiveBomb(position);}
		position.draw(board.IsColor() ? getColorForChar(position.getChar()) : WHITE);
		inventory[0] = ' ';
	}
	else {
		board.showMessage("No item to dispose.");
	}
}

void Player::clearFromScreen()
{
	state = false;
	position.setDirection(Keyboard_bind::STAY);
	board.setChar(position, ' ');
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
	Point originalPos = position;
	Point nextCandidate = position;
	nextCandidate.move();

	if (originalPos == nextCandidate)
		return true;

	char nextTile = board.getCharAt(nextCandidate);
	bool blocked = false;

	if (board.isWall(nextCandidate)) {
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
		position.setDirection(Keyboard_bind::STAY);
		return true; // stop further steps
	}
	else { // success: move to nextCandidate
		if (originalPos.getX() != nextCandidate.getX() || originalPos.getY() != nextCandidate.getY()) {
			if (board.getSpringAt(originalPos) == nullptr) {
				char underlying = board.getCharAt(originalPos);
				board.setChar(originalPos, underlying); // leaving no trail
			}
		}
		position = nextCandidate;
		position.draw(board.IsColor() ? getColorForChar(position.getChar()) : WHITE);
		return false; // can continue
	}
}

void Player::handleActiveSpring() {
	Spring* activeSpring = board.getSpringAt(position);
	// now we want to 'visualize' the spring if we are on one
	if (activeSpring != nullptr) {
		activeSpring->draw(position,board, true);
		Point checkWall = position;
		checkWall.move();
		if (board.isWall(checkWall)) {
			Sleep(200); //pause to show the spring effect
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

bool Player::handleSpecialObjects(char nextTile, Point nextPos, int force) {//function to handle special objects
	if (nextTile == objSigns::KEY) {
		if (addToInventory(objSigns::KEY,nextPos)) {
			// clear the key from the tile we are moving onto (nextPos), not the player's old position
			board.setChar(nextPos, ' ');
			return false;
		}
		return true; //if inventory is full its blocked
	}
	if (nextTile == objSigns::TORCH) {
		if (addToInventory(objSigns::TORCH, nextPos)) {
			board.setChar(nextPos, ' ');
			return false;
		}
		return true; //if inventory is full its blocked
	}
	if(nextTile == objSigns::RIDDLE){
		this->position.setDirection(Keyboard_bind::STAY); //try to make it stay when hit a riddle to avoid touching it several times in a row
		if (board.handleRiddle(nextPos, *this)){
			return false;
		}
		else {
			return true;
		}
	}
	if(nextTile == objSigns::BOMB){
		if (addToInventory(objSigns::BOMB, nextPos)) {
			board.setChar(nextPos, ' ');
			return false;
		}
		return true; //if inventory is full its blocked
	}
	if (isdigit((unsigned char)nextTile)) {  // check if it's a door
		if (board.isBombAt(nextPos))
			return true;
		return atDoor(nextTile, nextPos);
	}
		
	if (nextTile == objSigns::OBSTACLE) {
		Keyboard_bind pushDir = (springCyclesLeft > 0) ? springDir.getDirectionEnum() : position.getDirectionEnum();

		if (board.tryPushObstacle(nextPos, pushDir, force)) {
			return false;
		}
		return true;
	}
	return false;
}

void Player::reset(Point newPosition) {
	position = newPosition;
	state = true;
	position.setDirection(Keyboard_bind::STAY);
	springCyclesLeft = 0;
	currentForce = 1;
	finishedLevel = false;
	draw();
}

bool Player::atDoor(unsigned char nextTile, Point nextPos) {
	int doorId = nextTile - '0';
	if (board.ConnectionStatus(doorId)){ // connected to a switch
		if (board.SwitchState(doorId)) {return OpenDoorWithKey(doorId, nextPos);}//true = switch is on 
		else { //switch is off
			board.showMessage("The door is locked. Find the switch to open it.");
			return true;
		}
	}
	else { return OpenDoorWithKey(doorId, nextPos);}//not connected to a switch
	
}


bool Player::OpenDoorWithKey(int doorId, Point nextPos) {
	if (board.isDoorOpen(doorId)) {
		clearFromScreen();
		finishedLevel = true;
		return false;
	}
	else {
			int keyDoorId = board.GetDoorIdByKey(this->getChar());
			if(hasItem(objSigns::KEY) && keyDoorId == doorId){
			removeItem();
			board.showPlayerInfo(*this);
			if (board.isWinningDoor(doorId)) {
				Beep(700, 200);// for sound effect, from microsoft copilot
				if (board.getScore() < MIN_SCORE) {
					string msg = "You need at least " + std::to_string(MIN_SCORE) + " points to finish the game!";
					board.showMessage(msg);
					return true;
				}
				board.openDoor(doorId);
				clearFromScreen();
				board.showMessage("Correct door! Unlocked. + 100 points!");
				board.addScore(100);
				finishedLevel = true;
				return false;
			}
			else {
				board.setChar(nextPos, 'X');
				board.showMessage("Wrong door! It's a dead end. + 50 points! ");
				board.addScore(50);
				return true;
			}
		}
		else {
			board.showMessage("try look for the right key to unlock this door");
			return true;
		}
	}
}
int Player::getLives() const { return board.getLives(); }
int Player::getScore() const { return board.getScore(); }
void Player::decreaseLife() { board.decreaseLife(); }
void Player::increaseScore(int amount) { board.addScore(amount); }
bool Player::isAlive() const { return board.getLives() > 0; }
