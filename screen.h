#pragma once
//would contain screen related functions for the game map
#include "point.h"

class screen
{
public:
	enum { MAX_X = 80, MAX_Y = 25 };
private:
	//screen rectangle for 80x25 characters for the static objects
	const char* map[MAX_Y] = {
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"player 1 - $                                                player 2 - &        ", // 0
			"inventory:                                                  inventory:          ",  // 1
			"                                                                                ",  // 2
			"--------------------------------------------------------------------------------",  // 3
			"|                           |           |            |                         |",  // 4
			"|                           |           |            |                         |",  // 5
			"|                           |           |            |                         |",  // 6
			"|                                       |            |                         |",  // 7
			"|                                       |            |                         |",  // 8
			"|                           |           |            |                         |",  // 9
			"|                           |           |            |                         |",  // 10
			"|                           |           |            |                         |",  // 11
			"|                           |           |          ----------------------      |",  // 12
			"|                           |           |                                      |",  // 13
			"|                           |           |                                      |",  // 14
			"|                           |           |                                      |",  // 15
			"|                           |                                                  |",  // 16
			"|                           |                                                  |",  // 17
			"|                           |                                                  |",  // 18
			"|                           |           |                                      |",  // 19
			"|                           |           |                                      |",  // 20
			"|                           |           |                                      |",  // 21
			"|                           |           |                                      |",  // 22
			"|                           |           |                                      |",  // 23
			"--------------------------------------------------------------------------------"   // 24
	};

	char getCharAt(const point& p) const {
		return map[p.getY()][p.getX()];
	}
public:
	void draw(); //function to draw the map to the console
	//function to get the character at a specific position
	bool isWall(const point& p) const {
		return getCharAt(p) == '#';
	}

};