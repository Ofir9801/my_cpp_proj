#pragma once
#include "point.h"//using point class for player position
//class for each player's relevant data
int const INVENTORY_SIZE = 1; //player can hold up to one item
class player
{

private:
	point position; //player's position on the map
		//possible movement directions, will be edited for exact keys later
	char inventory[INVENTORY_SIZE]; //player can hold up to one item
	screen& screen; //reference to the game screen
public:
	Player::Player(const point& point, const char(&the_keys)[NUM_KEYS + 1], screen& theScreen): screen(theScreen);
	void move(); //function to move the player in the current direction
	void draw(); //function to draw the player on the map using gotoxy
	void handleKeyPressed(char key_pressed)
};

