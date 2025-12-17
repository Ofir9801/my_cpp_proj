#pragma once
#include "point.h"//using Point class for Player position
#include <iostream>
#include <string>

class Screen;//forward declaration to avoid circular dependency

class Player
{
	int springCyclesLeft = 0;
	int currentForce = 1; //acts as speed
	Point springDir; //direction of the flight
	Point position; //Player's position on the board
	char inventory[INVENTORY_SIZE + 1]; //Player can hold up to one item
	bool ExtraInventory = false; //flag to indicate if player has extra inventory slot
	string p_keys;
	Screen& board; //reference to the Game Screen
	bool state = true; //Player state - can move or not
	bool finishedLevel = false;
	size_t roomOpen = 0; //to track which room's door the player open
	bool handleSpecialObjects(char nextTile, Point nextPos, int force);
	int color = CYAN;

public:
	Player(const Point& point, const string(&keys), Screen& theScreen) :board(theScreen), position(point), p_keys(keys){
		inventory[0] = ' '; //empty inventory
		inventory[1] = ' '; //empty inventory
		inventory[2] = '\0'; //null terminator
	}
	void move(); //function to move the Player in the current direction
	void draw();
	void handleKeyPressed(char key_pressed);
	bool addToInventory(char item, Point position);
	char getChar() const { return position.getChar(); }
	const char* getInventory() const { return inventory; }
	bool hasItem(char item) const { return inventory[0] == item || inventory[1] == item; }
	//void removeItem();
	void dispose();
	void clearFromScreen();
	void reset(Point newPosition);
	void applySpringDirectionIfNeeded();
	int computeStepsToTake() const;
	bool takeStep();
	void handleActiveSpring();
	void finalizeMovement();
	Point getPosition() const { return position; }
	bool hasFinished() const { return finishedLevel; }
	bool atDoor(unsigned char nextTile, Point nextPos);
	bool OpenDoorWithKey(int doorId, Point nextPos);
	int getLives() const;
	int getScore() const;
	void decreaseLife();
	void increaseScore(int amount);
	bool isAlive() const;
	void setRoomOpen(size_t roomIndex) { roomOpen = roomIndex; };
	size_t getRoomOpen() const { return roomOpen; }
	bool OpenVaultRoom();
	bool OpenVictoryRoom();
	bool isExtraInventory() const { return ExtraInventory; }
	void setExtraInventory(bool val) { ExtraInventory = val; }
};
