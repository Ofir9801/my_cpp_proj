#include "Player.h"
#include "Screen.h"
#include "Spring.h"
#include <cctype>
#include <conio.h>
#include <windows.h>
#include "Game.h"

void Player::handleKeyPressed(char key_pressed, Game* g) {
	if (springCyclesLeft > 0) return; //disable changing direction during spring flight
	size_t index = 0;
	for (char k : p_keys) {
		if (std::tolower((unsigned char)k) == std::tolower(key_pressed)) {
			if(index != NUM_KEYS - 1){
				position.setDirection((Keyboard_bind)index);
				return;
			}
			else {
				dispose(g);
				return;
			}
		}
		++index;
	}
}

bool Player::addToInventory(objSigns item, Point pos)
{
	bool added = false;
	if (ExtraInventory) {
		for (int i = 0; i < INVENTORY_SIZE && !added; ++i) {
			if (inventory[i] == ' ') {
				inventory[i] = static_cast<char>(item); //add item to inventory
				added = true;
				if (item == objSigns::KEY)
					board.addKeyToInventory(pos, this->getChar());
			}
		}
	}
	else {
		if (inventory[0] == ' ') {
			inventory[0] = static_cast<char>(item); //add item to inventory
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

void Player::dispose(Game* g)
{
	int slotsCount = ExtraInventory ? INVENTORY_SIZE : 1;
	vector<int> occupiedSlots;
	for (int i = 0; i < slotsCount; ++i) {
		if (inventory[i] != ' ') {
			occupiedSlots.push_back(i);
		}
	}
	if(occupiedSlots.empty()) {
		board.showMessage("No item to dispose.");
		return;
	}
	int slotToDispose = occupiedSlots[0];
	if(occupiedSlots.size() > 1) {
		//ask player which item to dispose
		if (!board.IsSilent()) {
			board.showMessage("Press 1 to dispose first item, 2 to dispose second item.");
		}
		char choice = 0;
		while (true) {
			if (g->getInput(choice, g->getIteration())) {
				if (choice == '1' || choice == '2') {
					int index = choice - '1';
					if (std::find(occupiedSlots.begin(), occupiedSlots.end(), index) != occupiedSlots.end()) {
						slotToDispose = index;
						break;
					} else {
						if (!board.IsSilent()){
							board.showMessage("Invalid choice. Slot is empty. Press 1 or 2.");
						}
					}
				}
				else {
					if (!board.IsSilent()){
						board.showMessage("Invalid choice. Press 1 or 2.");
					}
				}
			}
		}
	}
	char c = inventory[slotToDispose];
	board.setChar(position, c);
	if (c == objSigns::KEY) { board.DisposeKeyToScreen(this->getChar(), position); }
	else if (c == objSigns::BOMB) { board.addActiveBomb(position); }
	drawToScreen();
	inventory[slotToDispose] = ' ';
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
	Spring* activeSpring = board.getSpringAt(position);
	if(activeSpring){
		activeSpring->interact(*this, board);
	}
	finalizeMovement();
}


void Player::applySpringDirectionIfNeeded() {
	if (springCyclesLeft > 0) {
		position.setDirection(springDir);
	}
}

void Player::SetSpringState(int force, Keyboard_bind direction)
{
	springCyclesLeft = force * force;
	currentForce = force;
	springDir = direction;
	position.setDirection(direction);
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
		if (nextTile != objSigns::OBSTACLE) {
			position.setDirection(Keyboard_bind::STAY);
		}
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
		drawToScreen();
		return false; // can continue
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
		if (board.getCurrentRoom() == roomIndex::VAULT) { return !board.handleVaultRiddle(nextPos); }//if its vault its a special case
		board.handleRiddle(nextPos, *this);
		return true;
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
		Keyboard_bind pushDir = (springCyclesLeft > 0) ? springDir : position.getDirectionEnum();
		int myForce = force;
		int partnerForce = board.getAssistForce(nextPos, pushDir, this);
		int totalForce = myForce + partnerForce;

		Obstacle* obs = board.getObstacleAt(nextPos);
		if (obs && obs->push(totalForce, pushDir)) {
			return false;
		}
		return true;
	}
	return false;
}

void Player::drawToScreen()
{
	if (board.IsSilent()) return;
	if (board.IsColor()) {
		// Assuming you have access to getColorForChar or similar logic
		position.draw(getColorForChar(position.getChar()));
	}
	else {
		position.draw(Color::WHITE);
	}
}

void Player::reset(Point newPosition) {
	position = newPosition;
	state = true;
	position.setDirection(Keyboard_bind::STAY);
	springCyclesLeft = 0;
	currentForce = 1;
	finishedLevel = false;
	drawToScreen();
}

bool Player::atDoor(unsigned char nextTile, Point nextPos){
	int doorId = nextTile - '0';
	if (board.getConnectionStatus(doorId)){ // connected to a switch
		if (board.SwitchState(doorId)) {return OpenDoorWithKey(doorId, nextPos);}//true = switch is on 
		else { //switch is off
			board.showMessage("The door is locked. Find the switch to open it.");
			return true;
		}
	}
	else if (doorId == roomIndex::VICTORY) { return OpenVictoryRoom(); }//In vault Room
	else { return OpenDoorWithKey(doorId, nextPos);}//not connected to a switch
	
}

void Player::RemoveKeyFromInventory(int keyDoorId) {
	for (int i = 0; i < INVENTORY_SIZE; ++i) {
		if (inventory[i] == objSigns::KEY) {
			board.RemoveKey(keyDoorId);
			inventory[i] = ' ';
			return;
		}
	}
}

bool Player::OpenDoorWithKey(int doorId, Point nextPos) {
	bool isOpenDoor = board.isDoorOpen(doorId);
	if (isOpenDoor) { //door is open
		if (doorId == roomIndex::VAULT) { return OpenVaultRoom(); }
		clearFromScreen();
		finishedLevel = true;
		roomOpen = doorId;
		return false;
	}
	int keyDoorId = board.GetDoorIdByKey(this->getChar());
	int keyDoorId2 = board.GetDoorIdByKey(this->getChar(),keyDoorId);
	bool RealDoor = board.isRealDoor(doorId);
	
	if (hasItem(objSigns::KEY) && (keyDoorId == doorId || keyDoorId2 == doorId)) {
		board.openDoor(doorId, position.getChar());
		board.DisposeKeyToScreen(this->getChar(), position);
		RemoveKeyFromInventory(keyDoorId);
		board.showPlayerInfo(*this);
		if (doorId == roomIndex::VAULT) { return OpenVaultRoom(); }
		if (RealDoor) {
			clearFromScreen();
			string msg = "The Door Is Unlocked, you get " + std::to_string(SUCCESS_SCORE) + " points!";
			board.showMessage(msg);
			board.addScore(SUCCESS_SCORE);
			finishedLevel = true;
			roomOpen = doorId;
			return false;
		}
		else { //fake door
			board.setChar(nextPos, FAKE_DOOR_CHAR);
			string msg = "It's a dead end, you get " + std::to_string(FAKE_DOOR_SCORE) + " points for trying";
			board.showMessage(msg);
			board.addScore(FAKE_DOOR_SCORE);
			return true;
		}
	}
	else {
		board.showMessage("Try to look for the right key to unlock this door");
		return true;
	}
}

bool Player::OpenVaultRoom() {
	int score = board.getScore();
	if (score >= MIN_SCORE) { //check if door is open and player has enough score to finish
		clearFromScreen();
		finishedLevel = true;
		roomOpen = static_cast<int>(roomIndex::VAULT);
		return false;
	}
		//alert the player that he needs more score to finish
		string msg = "You need at least " + std::to_string(MIN_SCORE) + " points to enter the vault!, you need more " + std::to_string(MIN_SCORE - score) + " points";
		board.showMessage(msg);
		return true;
}

bool Player::OpenVictoryRoom() {
	if (board.allRiddlesSolved()) {
		board.openDoor(static_cast<int>(roomIndex::VICTORY), position.getChar());
		clearFromScreen();
		finishedLevel = true;
		roomOpen = static_cast<int>(roomIndex::VICTORY);
		return false;
	}
	else {
		string msg = "You need to solve the riddle to proceed to the final screen";
		board.showMessage(msg);
		return true;
	}
}

void Player::decreaseLife() { board.decreaseLife(); }
void Player::increaseScore(int amount) { board.addScore(amount); }

