#pragma once
//would contain screen related functions for the game map
#include "point.h"
#include "player.h"
#include "objSigns.h"
class player; //forward declaration to avoid circular dependency

class screen{
public:
	enum { MAX_X = 80, MAX_Y = 25 };
	enum MESSAGES_POS { MES_X = 0, MES_Y = 2 };
private:
	//screen rectangle for 80x25 characters for the static objects
	char map[MAX_Y][MAX_X + 1];

	char getCharAt(const point& p) const {
		return map[p.getY()][p.getX()];
	}
public:
	screen();
	void loadMap(const char* arr[]); //function to load the map from a string array
	void draw(); //function to draw the map to the console
	//function to get the character at a specific position
	bool isWall(const point& p) const {
		char c = getCharAt(p);
		return c == '-' || c == '|';
	}
	void showPlayerInfo(player p);
	
	void showMessage(const char* msg) {
		gotoxy(MESSAGES_POS::MES_X, MESSAGES_POS::MES_Y);
		std::cout << msg << std::flush;
	}

};