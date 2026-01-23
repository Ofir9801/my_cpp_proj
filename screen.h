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
#include <filesystem>
#include <functional>
#include <random>

class Player; //forward declaration to avoid circular dependency
class Game;
using std::string, std::vector, std::map;

class Screen {
private:
	string board[MAX_Y];
	map<int,vector<string>> Rooms;
	string finalMessage = "";
	string currentSaveDirectory = "";
	int currentRoom = 0;
	vector<Spring> springs;
	vector<Switch> switches;
	map<int,Door>doors;
	vector<int>doorIDs; //keys to the doors map
	map<Point, Key> keys;
	map<Point, Riddle> riddles;
	map<Point, Obstacle> obstacles;
	vector<Bomb> activeBombs;
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
	friend class Obstacle;

	struct RoomState { //to save the state of each room
		bool visited = false;
		vector<string> layout;          
		vector<Spring> springs;         
		vector<Switch> switches;        
		map<Point,Obstacle> obstacles;
		vector<Bomb> activeBombs;
		map<Point, Riddle> riddles;
		map<Point, Key> keys;           
		map<int, Door> doors;	               
	};
	map <int,RoomState> savedRooms; //array to hold the state of each room (excluding menu ,instructions and victory)
	map <int, bool> roomDarkStatus; //to hold if the room is dark or not
	map<int, int> roomLegendRows;
	
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
	void resetStats() { sharedLives = STARTING_LIVES; sharedScore = 0; currentSaveDirectory = ""; }
    // game logic: Doors & Switches
	bool isDoorOpen(int door_id) const;
    void openDoor(int door_id, char player);
	bool getConnectionStatus(int door_id) const;
	bool isRealDoor(int doorId) const { return Rooms.find(doorId) != Rooms.end(); }
	bool SwitchState(int doorId) const;
	void handleSwitches(const Point& p1, const Point& p2);
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
	void saveGame();
	int loadGame(const string& file = "");
	string selectSaveFile();
	void cleanSavedGames();
	void setSaveDirectory(string dir) { currentSaveDirectory = dir; }
	
private:
	//gamecycle and initialization
	void initializeRooms();
	void linkDoorsToKeysAndSwitches();
	void loadItems(int doorIdOpen, Point& doorPos);
	string loadAllRooms();
	string loadRiddlesForCurrentRoom();
	void attemptFunctionWithRetry(std::function<string()> func); //use ai
	void CheckExplodeNecessaryObject(int doorId);
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
	string loadRoomState(int key, const string& filename, int& current); 
	void saveRoomState(const RoomState& state, const string& filename, const bool first) const;
	void setFileName(string& file, const int key, const string& folderPath) const;
	string getCurrentTimeStamp()const;
	string formatTime(std::filesystem::file_time_type ftime)const;
	//helpers
	int normalizeDoorId(int doorId) const {return (doorId == 9) ? 0 : doorId;}
	bool isGameRoom(int roomNum) { return roomNum >= 0; }
};