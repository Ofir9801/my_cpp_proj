#pragma once
#include "Point.h"
#include "Player.h"
#include "Spring.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Door.h"
#include "Key.h"
#include "Riddle.h"
#include "Bomb.h"
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
	std::vector<int>doorIDs;
	std::vector<Key> keys;
	std::vector<Riddle> riddles;
	std::vector<Bomb> activeBombs;
	bool colorToggle = false;
public:
	friend class Game;
	Screen();
	void loadMap(int roomNumber); //function to load the map from a string array
	void drawMap(); 
	void drawMap(int roomNumber); //function to draw the map to the console
	//function to get the character at a specific position
	bool isWall(const Point& p) const;
	size_t getCurrentRoom() const { return currentRoom; }
	void showPlayerInfo(const Player& p);
	void initaializeRoomsArray();
	void showMessage(const char* msg);
	char getCharAt(const Point& p) const { return map[p.getY()][p.getX()]; }
	void setChar(const Point& p, char c);//function to set a character on Screen at Point p, like picking up a key
	void showKeyBinds(const char* keys1, const char* keys2)const;
	bool tryPushObstacle(const Point& obstaclePos, Keys direction, int force);
	void loadSprings();
	Spring* getSpringAt(const Point& p);
	void refreshSpringsDisplay(const Point& p1, const Point& p2) const;
	void clearMessegeArea(int const counter);
	bool isOnOpenDoor(const Point& p) const { return getCharAt(p) == '{'; }
	bool isWinningDoor(int doorId) const { return doorId == currentRoom - 1; }
	void loadItems();
	void linkDoorsToKeysAndSwitches();
	bool isDoorOpen(int door_id);
	void openDoor(int door_id);
	void setconnection(int door_id);
	bool ConnectionStatus(int doorId);
	bool SwitchState(int doorId);
	bool IsColor() const { return colorToggle; }
	void addKeyToInventory(Point position, char p);
	void RemoveKeyFromInventory(char p, Point newPos);
	int GetDoorIdByKey(char p);
	void updateBombs(Player& p1, Player& p2);
	bool handleRiddle(const Point& p, Player& player);
	bool isValid(const Point& p) const {
		return p.getX() >= 0 && p.getX() < MAX_X &&p.getY() >= 0 && p.getY() < MAX_Y;
	}
	void addActiveBomb(const Point& p) { activeBombs.push_back(Bomb(p)); }
};