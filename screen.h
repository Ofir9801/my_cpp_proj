#pragma once
//would contain screen related functions for the game map
#include "point.h";

class screen
{
public:
	enum { MAX_X = 80, MAX_Y = 25 };
private:
	//screen rectangle for 80x25 characters for the static objects
	char* map[MAX_Y] = {};

	char getCharAt(const point& p) const {
		return map[p.getY()][p.getX()];
	}
public:
	screen(); //default constructor to initialize the map with no objects
	void draw(); //function to draw the map to the console
	//function to get the character at a specific position
	bool isWall(const point& p) const {
		return getCharAt(p) == '#';
	}

};