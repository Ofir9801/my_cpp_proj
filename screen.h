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
	int sharedLives = STARTING_LIVES;
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
	
	//gamecycle and initialization
    Screen();
    void loadMap(int roomNumber); // Loads board from string array
    void loadSprings();
	void saveRoom();
	void setGameState(bool state) { gameState = state; }
	void clearSavedRooms() { savedRooms.clear(); }
    // display
    void drawMap();
    void refreshSpringsDisplay(const Point& p1, const Point& p2) const;
    void showInstructionBinds() const;
    bool IsColor() const { return colorToggle; }
    // UI
    void showPlayerInfo(const Player& p);
    void showMessage(string msg);
    void clearMessegeArea();
	// general board functions
    char getCharAt(const Point& p) const { return board[p.getY()][p.getX()]; }
    void setChar(const Point& p, char c);
	void setChar(const Point& p, objSigns sign);
    bool isWall(const Point& p) const;
    bool isValid(const Point& p) const;
    size_t getCurrentRoom() const { return currentRoom; }
    // lighting system
    bool isDark() const { return isDarkRoom; }
    void updateLighting(const Point& p1, const Point& p1Prev, const Player& player1,
						const Point& p2, const Point& p2Prev, const Player& player2);
	bool BoxDistance(int x, int y, const Point& p, int radius) const;
	bool RoundDistance(int x, int y, const Point& p, int radius) const;
    // game state score & lives
    void addScore(int amount) { sharedScore += amount; }
    int getScore() const { return sharedScore; }
    int getLives() const { return sharedLives; }
    void decreaseLife();
    void resetStats() { sharedLives = 4; sharedScore = 0; }
    // game logic: Doors & Switches
	bool isDoorOpen(int door_id) const;
    void openDoor(int door_id);
	bool getConnectionStatus(int door_id) const;
	bool isRealDoor(int doorId) const { return doorId >= roomIndex::VAULT && doorId <= roomIndex::VICTORY; }
	bool SwitchState(int doorId) const;
	int GetDoorIdByKey(char p) const;
    void deleteDoor(Point position);
    void deleteSwitch(Point position);
    // game logic: Keys & Inventory
    void addKeyToInventory(Point position, char p);
	void RemoveKey(int doorId);
    void DisposeKeyToScreen(char p, Point newPos);
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
	bool allRiddlesSolved() const;
	bool handleVaultRiddle(Point riddlePos);
private:
	//gamecycle and initialization
	void initaializeRoomsArray();
	void linkDoorsToKeysAndSwitches();
	void loadItems(int doorIdOpen);
	void loadRiddles();
	// display
	void drawVictoryRoom();
	// UI
	string CreateInventoryDisplay(const Player& p);
	// lighting system
	void ProcessLightning(int cx, int cy, int radius, bool erase, const Point& p1, const Point& p2, const int r1, const int r2);
	// game logic:Switches
	void setconnection(int door_id);
	// game logic: Riddles
	Riddle ReadRiddleFromFile(const string& filePath, const Point pos, int riddleIndex, bool& error);
	Riddle ReadVaultRiddleFromFile(const string& filePath, const Point pos, bool& error);
	void CheckExplodeNecessaryObject(int doorId);
};