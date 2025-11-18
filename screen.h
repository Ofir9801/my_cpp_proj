#pragma once
//would contain screen related functions for the game map
#include "point.h"

class screen
{
public:
	enum { MAX_X = 80, MAX_Y = 25 };
private:
	//screen rectangle for 80x25 characters for the static objects
	const char* map[MAX_Y + 1] = {
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"WWWWWWWWWWWWWWWW   WWWWWWWWWWWW   WWWWWWWWWWWW  WWWWWWWWWWWW  WWWWWWWWWWWWWWWW W", // 0
			"W                           W           W            W                         W",  // 1
			"W                           W           W            W                         W",  // 2
			"W                           W           W            W                         W",  // 3
			"W                           W           W            W                         W",  // 4
			"W                           W           W            W                         W",  // 5
			"W                           W           W            W                         W",  // 6
			"W                                       W            W                         W",  // 7
			"W                                       W            W                 %       W",  // 8
			"W                           W           W            W                         W",  // 9
			"W                           W           W            W                         W",  // 10
			"W                           W           W            W                         W",  // 11
			"W                           W           W      WWWWWWWWWWWWWWWWWWWWWWWWWW      W",  // 12
			"W                           W           W                                      W",  // 13
			"W                           W           W                                      W",  // 14
			"W                           W           W                                      W",  // 15
			"W                           W                                                  W",  // 16
			"W                           W                                                  W",  // 17
			"W                           W                                                  W",  // 18
			"W                           W           W                                      W",  // 19
			"W                           W           W                                      W",  // 20
			"W                           W           W                                      W",  // 21
			"W                           W           W                                      W",  // 22
			"W                           W           W                                      W",  // 23
			"WWWWWWWWWWWWWWWW   WWWWWWWWWWWW   WWWWWWWWWWWW  WWWWWWWWWWWW  WWWWWWWWWWWWWWWW W"   // 24
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