#pragma once
#include "point.h"
#include "Player.h"
#include "Spring.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Door.h"
#include <vector>

class Player; //forward declaration to avoid circular dependency

class Screen {
private:
	//Screen rectangle for 80x25 characters for the static objects
	char map[MAX_Y][MAX_X + 1];
	const char** Rooms[NUM_ROOMS];
	size_t currentRoom = 0;
	std::vector<Spring> springs;
	std::vector<Switch> switches;
	std::vector<Obstacle> obstacles;
	std::vector<Door> doors;
	

public:
	friend class Game;
	Screen();
	void loadMap(int roomNumber); //function to load the map from a string array
	void drawMap(); //function to draw the map to the console
	//function to get the character at a specific position
	bool isWall(const point& p) const;
	size_t getCurrentRoom() const { return currentRoom; }
	void showPlayerInfo(const Player& p);
	void initaializeRoomsArray();
	void showMessage(const char* msg);
	char getCharAt(const point& p) const { return map[p.getY()][p.getX()]; }
	void setChar(const point& p, char c);//function to set a character on Screen at point p, like picking up a key
	void showKeyBinds(const char* keys1, const char* keys2)const;
	bool tryPushObstacle(const point& obstaclePos, Keys direction, int force);
	void loadSprings();
	Spring* getSpringAt(const point& p);
	void refreshSpringsDisplay(const point& p1, const point& p2) const;
	void clearMessegeArea(int const counter);
	bool isOnOpenDoor(const point& p) const { return getCharAt(p) == '{'; }
	bool isWinningDoor(int doorId) const { return doorId == currentRoom - 1; }
	void loadItems();
	void autoLinkSwitchesAndDoors();
	//void updateSwitches();
	bool isDoorOpen(int door_id);
	void openDoor(int door_id);
	void setconnection(int door_id);
	bool ConnectionStatus(int doorId);
	bool SwitchState(int doorId);
};