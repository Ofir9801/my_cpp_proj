#pragma once
#include "point.h"//using point class for player position
//class for each player's relevant data
int const INVENTORY_SIZE = 1; //player can hold up to one item
class player
{
public:
	enum direction { UP, DOWN, LEFT, RIGHT, STAY };
private:
	point position; //player's position on the map
	direction dir;
	//possible movement directions, will be edited for exact keys later
	char symbol; //character symbol representing the player on the map
	char inventory[INVENTORY_SIZE]; //player can hold up to one item
public:
	player(int startX = 1, int startY = 1, char pSymbol = '@') { //default constructor
		position = point(startX, startY); //default position at (1,1)
		symbol = pSymbol; //default symbol for the player
		dir = STAY; //default direction
	}
	void setDirection(direction newDir) {
		dir = newDir;
	}
	void move(); //function to move the player in the current direction
	void draw(); //function to draw the player on the map using gotoxy
	void stop() { setDirection(STAY); } //function to stop the player's movement

};

