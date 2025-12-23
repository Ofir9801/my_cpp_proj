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
#include <map>

class Player; //forward declaration to avoid circular dependency
using std::string;

class Screen {
private:
	//Screen rectangle for 80x25 characters for the static objects
	string board[MAX_Y];
	string* Rooms[NUM_ROOMS];
	size_t currentRoom = 0;
	std::vector<Spring> springs;
	std::vector<Switch> switches;
	std::vector<Obstacle> obstacles;
	std::map<int,Door>doors;
	std::vector<int>doorIDs; //keys to the doors map
	std::map<Point, Key> keys;
	std::vector<Riddle> riddles;
	std::vector<Bomb> activeBombs;
	bool colorToggle = false;
	bool isDarkRoom = false;
	bool gameState = true;
	int sharedLives = 4;
	int sharedScore = 0;
public:
	friend class Game;
	//gamecycle and initialization
    Screen();
    void initaializeRoomsArray();
    void linkDoorsToKeysAndSwitches();
    void loadMap(int roomNumber); // Loads board from string array
    void loadItems();
    void loadSprings();
    // display
    void drawMap();
    void drawMap(int roomNumber);
    void refreshSpringsDisplay(const Point& p1, const Point& p2) const;
    void showKeyBinds() const;
    bool IsColor() const { return colorToggle; }
    // UI
    void showPlayerInfo(const Player& p);
    void showMessage(string msg);
    void clearMessegeArea(int const counter);
	// general board functions
    char getCharAt(const Point& p) const { return board[p.getY()][p.getX()]; }
    void setChar(const Point& p, char c);
    bool isWall(const Point& p) const;
    bool isValid(const Point& p) const;
    size_t getCurrentRoom() const { return currentRoom; }
    // lighting system
    bool isDark() const { return isDarkRoom; }
    void updateLighting(const Point& p1, const Point& p1Prev, const Player& player1,
        const Point& p2, const Point& p2Prev, const Player& player2);
    void ProcessLightning(int cx, int cy, int radius, bool erase, const Point& p1, const Point& p2, const int r1, const int r2);
    bool Distance(int x, int y, const Point& p, int r);
    // game state (score & lives)
    void addScore(int amount) { sharedScore += amount; }
    int getScore() const { return sharedScore; }
    int getLives() const { return sharedLives; }
    void decreaseLife();
    void resetStats() { sharedLives = 4; sharedScore = 0; }
    // game logic: Doors & Switches
    bool isDoorOpen(int door_id);
    void openDoor(int door_id);
    bool isOnOpenDoor(const Point& p) const { return getCharAt(p) == '{'; }
    bool isWinningDoor(int doorId) const { return doorId == currentRoom - 1; }
    void setconnection(int door_id);
    bool ConnectionStatus(int door_id);
    bool SwitchState(int doorId);
    int GetDoorIdByKey(char p);
    void deleteDoor(Point position);
    void deleteSwitch(Point position);
    // game logic: Keys & Inventory
    void addKeyToInventory(Point position, char p);
    void RemoveKeyFromInventory(char p, Point newPos);
    void deleteKey(Point position);
    // game logic: Bombs & Obstacles
    void updateBombs(Player& p1, Player& p2);
    void addActiveBomb(const Point& p) { activeBombs.push_back(Bomb(p)); }
    bool isBombAt(const Point& p) const;
	Obstacle* getObstacleAt(const Point& p);
    // game logic: Springs & Riddles
    Spring* getSpringAt(const Point& p);
    void deleteSpring(Point position);
    bool handleRiddle(Point riddlePos, Player& player);
};