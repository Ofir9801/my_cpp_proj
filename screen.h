#pragma once
//would contain Screen related functions for the Game map
#include "point.h"
#include "Player.h"
#include "objSigns.h"
class Player; //forward declaration to avoid circular dependency

int constexpr NUM_ROOMS = 3; //number of rooms in the Game

class Screen{
public:
	enum { MAX_X = 80, MAX_Y = 25 };
	enum MESSAGES_POS { MES_X = 0, MES_Y = 2 };
	
private:
	//Screen rectangle for 80x25 characters for the static objects
	char map[MAX_Y][MAX_X + 1];
	const char** Rooms[NUM_ROOMS]; 


public:
	Screen();
	void loadMap(const char* arr[]); //function to load the map from a string array
	void draw(); //function to draw the map to the console
	//function to get the character at a specific position
	bool isWall(const point& p) const;
	void showPlayerInfo(Player p);
	void initaializeRoomsArray();
	void showMessage(const char* msg) {
		gotoxy(MESSAGES_POS::MES_X, MESSAGES_POS::MES_Y);
		std::cout << msg << std::flush;
	}
	char getCharAt(const point& p) const {
		return map[p.getY()][p.getX()];
	}
	void setChar(const point& p, char c);//function to set a character on Screen at point p, like picking up a key

};