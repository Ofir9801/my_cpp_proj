#pragma once
#include "Point.h"
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
	std::map<Point, Riddle> riddles;
	std::vector<Bomb> activeBombs;
	bool colorToggle = false;
	bool isDarkRoom = false;
	bool gameState = true;
	int sharedLives = 4;
	int sharedScore = 0;
	int MessageTimer = 0;

	struct RoomState { //to save the state of each room
		std::vector<string> layout;          
		std::vector<Spring> springs;         
		std::vector<Switch> switches;        
		std::vector<Obstacle> obstacles;     
		std::map<int, Door> doors;           
		std::map<Point, Key> keys;           
		std::vector<Bomb> activeBombs;       
		std::map<Point, Riddle> riddles;     
		bool visited = false;                
	};
	std::map <int,RoomState> savedRooms; //array to hold the state of each room (excluding menu ,instructions and victory)

public:
	friend class Game;
	Screen();
	void loadMap(int roomNumber); 
	void drawMap(); 
	void drawMap(int roomNumber); 
	bool isWall(const Point& p) const;
	size_t getCurrentRoom() const { return currentRoom; }
	void showPlayerInfo(const Player& p);
	string BuildInventory(const Player& p);
	void initaializeRoomsArray();
	void showMessage(string msg);
	char getCharAt(const Point& p) const {return board[p.getY()][p.getX()];}
	void setChar(const Point& p, char c);
	void showKeyBinds()const;
	bool tryPushObstacle(const Point& obstaclePos, Keyboard_bind direction, int force);
	void loadSprings();
	Spring* getSpringAt(const Point& p);
	void refreshSpringsDisplay(const Point& p1, const Point& p2) const;
	void clearMessegeArea();
	bool isOnOpenDoor(const Point& p) const { return getCharAt(p) == '{'; }
	bool isRealDoor(int doorId) const { return doorId >= roomIndex::VICTORY && doorId <=roomIndex::VAULT; }
	void loadItems(int doorIdOpen);
	void linkDoorsToKeysAndSwitches();
	bool isDoorOpen(int door_id) const;
	void openDoor(int door_id);
	void setconnection(int door_id);
	bool getConnectionStatus(int door_id) const;
	bool SwitchState(int doorId) const;
	bool IsColor() const { return colorToggle; }
	void addKeyToInventory(Point position, char p);
	void RemoveKeyFromInventory(char p, Point newPos);
	int GetDoorIdByKey(char p) const;
	void updateBombs(Player& p1, Player& p2);
	bool handleRiddle(Point riddlePos, Player &player);
	bool isDark() const { return isDarkRoom; }
	void updateLighting(const Point& p1, const Point& p1Prev, const Player& player1,
						const Point& p2, const Point& p2Prev, const Player& player2);
	bool Distance(int x, int y, const Point& p, int r) const;
	void ProcessLightning(int cx, int cy, int radius, bool erase, const Point& p1, const Point& p2, const int r1, const int r2);
	bool isValid(const Point& p) const;
	void addActiveBomb(const Point& p) { activeBombs.push_back(Bomb(p)); }
	void deleteKey(Point position);
	void deleteSpring(Point position);
	void deleteSwitch(Point position);
	void deleteDoor(Point position);
	bool isBombAt(const Point& p)const;
	void addScore(int amount) { sharedScore += amount; }
	int getScore() const { return sharedScore; }
	int getLives() const { return sharedLives; }
	void decreaseLife();
	void resetStats() { sharedLives = 4; sharedScore = 0; }
	void loadRiddles();
	Riddle ReadRiddleFromFile(const string& filePath,const Point pos, int riddleIndex, bool& error);
	bool allRiddlesSolved() const;
	void saveRoom();
	void clearSavedRooms() {savedRooms.clear();}
	bool handleVaultRiddle(Point riddlePos);
	Riddle ReadVaultRiddleFromFile(const string& filePath, const Point pos, bool& error);
};