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
#include <random>

class Player; //forward declaration to avoid circular dependency
class Game;
using std::string;

class Screen {
private:
	string board[MAX_Y];
	string* Rooms[NUM_ROOMS];
	string finalMessage = "";
	int currentRoom = 0;
	std::vector<Spring> springs;
	std::vector<Switch> switches;
	//std::vector<Obstacle> obstacles;
	std::map<int,Door>doors;
	std::vector<int>doorIDs; //keys to the doors map
	std::map<Point, Key> keys;
	std::map<Point, Riddle> riddles;
	std::map<Point, Obstacle> obstacles;
	std::vector<Bomb> activeBombs;
	std::vector<string>RiddlePathWays;
	bool colorToggle = false;
	bool isDarkRoom = false;
	bool gameState = true;
	bool isSilent = false;
	int sharedLives = STARTING_LIVES;
	int sharedScore = 0;
	int MessageTimer = 0;
	std::mt19937 rng;
	unsigned int currentSeed;
	Game* game = nullptr;

	struct RoomState { //to save the state of each room
		bool visited = false;
		std::vector<string> layout;          
		std::vector<Spring> springs;         
		std::vector<Switch> switches;        
		std::map<Point,Obstacle> obstacles;
		std::vector<Bomb> activeBombs;
		std::map<Point, Riddle> riddles;
		std::map<Point, Key> keys;           
		std::map<int, Door> doors;	               
	};
	std::map <int,RoomState> savedRooms; //array to hold the state of each room (excluding menu ,instructions and victory)
	
public:
	friend class Game;
	
	//gamecycle and initialization
    Screen(unsigned int seed = 0);
    void loadMap(int roomNumber, Point& doorPos); // Loads board from string array
    void loadSprings();
	void saveRoom();
	void setGameState(bool state) { gameState = state; }
	void clearSavedRooms() { savedRooms.clear(); }
	void setSeed(unsigned int seed);
	unsigned int getSeed () const { return currentSeed; }
	void setGame(Game* g) { game = g; }
	Game* getGame(){ return game; }
	void setSilentMode(bool silent) { isSilent = silent; }
	bool IsSilent()const { return isSilent; }
    // display
    void drawMap();
    void refreshSpringsDisplay(const Point& p1, const Point& p2) const;
    void showInstructionBinds() const;
    bool IsColor() const { return colorToggle; }
	string getFinalMessage() { return finalMessage; }
    // UI
    void showPlayerInfo(const Player& p);
    void showMessage(string msg);
    void clearMessegeArea(bool forceClean = false);
	// general board functions
    char getCharAt(const Point& p) const { return board[p.getY()][p.getX()]; }
    void setChar(const Point& p, char c);
	void setChar(const Point& p, objSigns sign);
    bool isWall(const Point& p) const;
    bool isValid(const Point& p) const;
    int getCurrentRoom() const { return currentRoom; }
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
	void resetStats() { sharedLives = STARTING_LIVES; sharedScore = 0; }
    // game logic: Doors & Switches
	bool isDoorOpen(int door_id) const;
    void openDoor(int door_id, char player);
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
	void deleteObstacle(Point position);
    // game logic: Springs & Riddles
    Spring* getSpringAt(const Point& p);
    void deleteSpring(Point position);
	void deleteRiddle(Point position);
    bool handleRiddle(Point riddlePos, Player& player);
	bool allRiddlesSolved() const;
	bool handleVaultRiddle(Point riddlePos);
	//read and write from Files functions
	void saveGame() const;
	int loadGame();
	
private:
	//gamecycle and initialization
	void initializeRoomsArray();
	void linkDoorsToKeysAndSwitches();
	void loadItems(int doorIdOpen, Point& doorPos);
	string loadRiddles();
	void CheckExplodeNecessaryObject(int doorId);
	void loadFilesByType(bool type);
	bool isDestructible(const Point& p)const;
	bool inLegendBounds(const int legendY, const int y) const;
	// display
	void drawVictoryRoom();
	// UI
	string CreateInventoryDisplay(const Player& p);
	// lighting system
	void ProcessLightning(int cx, int cy, int radius, bool erase, const Point& p1, const Point& p2, const int r1, const int r2);
	// game logic:Switches
	void setConnection(int door_id);
	// game logic: Riddles
	Riddle ReadRiddleFromFile(const string& filePath, const Point pos, int riddleIndex, string& errorMsg);
	Riddle ReadVaultRiddleFromFile(const string& filePath, const Point pos, string& errorMsg);
	//read and write from Files functions
	std::string loadRoomState(int key, const std::string& filename, int& current); //change to string for error handling
	void saveRoomState(const RoomState& state, const std::string& filename, const bool first) const;
	void setFileName(std::string& file, const int key) const;
};