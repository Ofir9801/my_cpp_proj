#pragma once
#include "point.h"//using point class for Player position
#include "Screen.h"//using Screen class to check for walls	

class Screen;//forward declaration to avoid circular dependency


class Player
{
	int springCyclesLeft = 0;
	int currentForce = 1; //acts as speed
	point springDir; //direction of the flight
	point position; //Player's position on the map
	char inventory[INVENTORY_SIZE+1]; //Player can hold up to one item
	char p_keys[NUM_KEYS];
	Screen& map; //reference to the Game Screen
	bool state = true; //Player state - can move or not
	bool handleSpecialObjects(char nextTile, point nextPos, int force);

public:
	Player(const point& point, const char(&the_keys)[NUM_KEYS + 1], Screen& theScreen) :map(theScreen) {
		memcpy(p_keys, the_keys, NUM_KEYS * sizeof(p_keys[0]));
		position = point;
		inventory[0] = ' '; //empty inventory
		inventory[INVENTORY_SIZE] = '\0'; //null-terminate the inventory string
	}
	void move(); //function to move the Player in the current direction
	void draw() {
		position.draw();
	}
	//function to draw the Player on the map using gotoxy
	void handleKeyPressed(char key_pressed);
	bool addToInventory(char item);
	char getChar() const { return position.getChar(); }
	const char* getInventory() const { return inventory; }
	bool hasItem(char item) const { return inventory[0] == item; }
	void removeItem() { inventory[0] = ' '; }
	void dispose();
	void clearFromScreen();
	void reset(point newPosition);
	void applySpringDirectionIfNeeded();
	bool checkAndHandleThroughDoor();
	int computeStepsToTake() const;
	bool takeStep();
	void handleActiveSpring();
	void finalizeMovement();
	point getPosition() const { return position; }
	Keys getOppositeDirection(Keys dir);
	
};
