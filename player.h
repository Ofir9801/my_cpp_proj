#pragma once
#include "point.h"//using point class for player position
#include "screen.h"//using screen class to check for walls	
//class for each player's relevant data
int const INVENTORY_SIZE = 1; //player can hold up to one item
int const NUM_KEYS = 6; //number of possible movement keys
class player
{
	point position; //player's position on the map
	char inventory[INVENTORY_SIZE]; //player can hold up to one item
	char p_keys[NUM_KEYS];
	screen& map; //reference to the game screen
public:	
	player(const point& point, const char(&the_keys)[NUM_KEYS + 1], screen& theScreen) :map(theScreen) {
		memcpy(p_keys, the_keys, NUM_KEYS * sizeof(p_keys[0]));
		position = point;
		inventory[0] = ' '; //empty inventory
	}
	void move(); //function to move the player in the current direction
	void draw() {
		position.draw();
	}
		//function to draw the player on the map using gotoxy
	void handleKeyPressed(char key_pressed);
};

