#pragma once
//would contain Screen related functions for the Game map
#include "point.h"
#include "Player.h"
#include "objSigns.h"
class Player; //forward declaration to avoid circular dependency

int constexpr NUM_ROOMS = 4; //number of rooms in the Game
enum roomIndex { MENU = 0, INSTRUCTIONS = 1, ROOM1 = 2, ROOM2 = 3 };

class Screen{
public:
	enum { MAX_X = 80, MAX_Y = 25 };
	enum MESSAGES_POS { MES_X = 0, MES_Y = 2 };
	
private:
	//Screen rectangle for 80x25 characters for the static objects
	char map[MAX_Y][MAX_X + 1];
	const char** Rooms[NUM_ROOMS]; 
	size_t currentRoom = 0;
	bool successfulMove = false;


public:
	Screen();
	void loadMap(int roomNumber); //function to load the map from a string array
	void drawMap(); //function to draw the map to the console
	//function to get the character at a specific position
	bool isWall(const point& p) const;
	size_t getCurrentRoom() const { return currentRoom; }
	void showPlayerInfo(const Player& p);
	void initaializeRoomsArray();
	void showMessage(const char* msg);
	char getCharAt(const point& p) const {return map[p.getY()][p.getX()];}
	void setChar(const point& p, char c);//function to set a character on Screen at point p, like picking up a key
	void showKeyBinds(const char* keys1, const char* keys2)const;
	void room1Challenge(char ch,point position, Player* p);
	void setSuccessfulMove(bool val) { successfulMove = val; }
	bool getSuccessfulMove() const { return successfulMove; }
};