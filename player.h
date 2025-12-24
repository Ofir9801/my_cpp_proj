#pragma once
#include "point.h"//using Point class for Player position
#include <iostream>
#include <string>

class Screen;//forward declaration to avoid circular dependency

class Player
{
	Point position; //Player's position on the board
	string p_keys;
	bool state = true; //Player state - can move or not
	bool finishedLevel = false;
	size_t roomOpen = 0; //to track which room's door the player open
	Color color = Color::CYAN;
	int springCyclesLeft = 0;
	int currentForce = 1; //acts as speed
	char inventory[INVENTORY_SIZE + 1]; 
	bool ExtraInventory = false; //flag to indicate if player has extra inventory slot
	Screen& board; //reference to the Game Screen

	bool handleSpecialObjects(char nextTile, Point nextPos, int force);
	Point springDir; //direction of the flight
	

public:
	//ctor
	Player(const Point& point, const string(&keys), Screen& theScreen) :board(theScreen), position(point), p_keys(keys) {
		inventory[0] = ' '; //empty inventory
		inventory[1] = ' '; //empty inventory
		inventory[2] = '\0'; //null terminator
	}
	//handle player movement
	void move(); //function to move the Player in the current direction
	void draw();
	void handleKeyPressed(char key_pressed);
	int computeStepsToTake() const;
	bool takeStep();
	void reset(Point newPosition);
	void finalizeMovement();
	void clearFromScreen();

	//Inventory Handling
	const char* getInventory() const { return inventory; }
	bool hasItem(objSigns item) const { return inventory[0] == item || inventory[1] == item; }
	void resetInventory() { inventory[0] = ' '; inventory[1] = ' '; inventory[2] = '\0'; }
	bool addToInventory(objSigns item, Point pos);
	void dispose();
	bool isExtraInventory() const { return ExtraInventory; }
	void setExtraInventory(bool val) { ExtraInventory = val; }

	//get and set player data
	int getLives() const;
	int getScore() const;
	void decreaseLife();
	void increaseScore(int amount);
	bool isAlive() const;
	char getChar() const { return position.getChar(); }
	Point getPosition() const { return position; }
	bool hasFinished() const { return finishedLevel; }
	void setRoomOpen(size_t roomIndex) { roomOpen = roomIndex; };
	roomIndex getRoomOpen() const { return (roomIndex)roomOpen; }

	//interaction with other objects
	bool atDoor(unsigned char nextTile, Point nextPos);
	bool OpenDoorWithKey(int doorId, Point nextPos);
	bool OpenVaultRoom();
	bool OpenVictoryRoom();

	//spring handlement
	void applySpringDirectionIfNeeded();
	void handleActiveSpring();



};
