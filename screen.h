#pragma once
#include "Point.h"
#include "Spring.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Door.h"
#include "Key.h"
#include "Riddle.h"
#include "LightingSystem.h"
#include "Bomb.h"
#include "roomState.h"
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
	map<int, vector<string>> Rooms;
	map <int, RoomState> savedRooms; //array to hold the state of each room (excluding menu ,instructions and victory)
	map <int, bool> roomDarkStatus; //to hold if the room is dark or not
	map<int, int> roomLegendRows;
	map<int, Door>doors;
	map<Point, Key> keys;
	map<Point, Riddle> riddles;
	map<Point, Obstacle> obstacles;
	vector<Spring> springs;
	vector<Switch> switches;
	vector<int>doorIDs; //keys to the doors map
	vector<Bomb> activeBombs;
	Game* game = nullptr;
	Player* player1 = nullptr;
	Player* player2 = nullptr;
	LightingSystem lighting;
	string finalMessage = "";
	string currentSaveDirectory = "";

	std::mt19937 rng;
	bool colorToggle = false;
	bool gameState = true;
	bool isSilent = false;
	unsigned int currentSeed;
	int currentRoom = 0;
	int sharedLives = STARTING_LIVES;
	int sharedScore = 0;
	int MessageTimer = 0;

public:
	friend class Game;
	friend class Obstacle;
	//gamecycle and initialization
	Screen(unsigned int seed = 0);
	void loadMap(int roomNumber, Point& doorPos); // Loads board from string array
	void loadSprings();
	void saveRoom();
	void setGameState(bool state) { gameState = state; }
	void clearSavedRooms() { savedRooms.clear(); }
	void setSeed(unsigned int seed);
	unsigned int getSeed() const { return currentSeed; }
	void setGame(Game* g) { game = g; }
	void setPlayers(Player* p1, Player* p2) { player1 = p1; player2 = p2; }
	Game* getGame() { return game; }
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
	bool BoxDistance(int x, int y, const Point& p, int radius) const;
	int getLegendY() const;
	bool inLegendBounds(const int legendY, const int y) const;
	//lighting system
	bool isDark() const { return lighting.IsDark(); }
	void updateLighting(const Point& p1, const Point& p1Prev, const Player& currentPlayer1,
		const Point& p2, const Point& p2Prev, const Player& currentPlayer2) {
		lighting.updateLighting(p1, p1Prev, currentPlayer1, p2, p2Prev, currentPlayer2);
	}
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
	int getAssistForce(Point obstaclePos, Keyboard_bind dir, const Player* whoIsAsking);
	bool isSameObstacleGroup(Point p1,Point p2);
    // game logic: Springs & Riddles
    Spring* getSpringAt(const Point& p);
    void deleteSpring(Point position);
	void deleteRiddle(Point position);
    bool handleRiddle(Point riddlePos, Player& player);
	bool allRiddlesSolved() const;
	bool handleVaultRiddle(Point riddlePos);
	//read and write from Files functions
	string selectSavedFile();
	void saveGame();
	int loadGame(const string& filepath = "");
	void cleanSavedGames();
	void setSaveDirectory(string dir) { currentSaveDirectory = dir; }
	
private:
	//gamecycle and initialization
	void linkDoorsToKeysAndSwitches();
	void loadItems(int doorIdOpen, Point& doorPos);
	string loadAllRooms();
	string loadRiddlesForCurrentRoom();
	void attemptFunctionWithRetry(std::function<string()> func); //use ai
	void CheckExplodeNecessaryObject(int doorId);
	bool isDestructible(const Point& p)const;
	// display
	void drawVictoryRoom();
	// UI
	string CreateInventoryDisplay(const Player& p);
	// game logic:Switches
	void setConnection(int door_id);
	// game logic: Riddles
	Riddle ReadRiddleFromFile(const string& filePath, const Point pos, int riddleIndex, string& errorMsg);
	Riddle ReadVaultRiddleFromFile(const string& filePath, const Point pos, string& errorMsg);
	//helpers
	int normalizeDoorId(int doorId) const {return (doorId == 9) ? 0 : doorId;}
	bool isGameRoom(int roomNum) { return roomNum >= 0; }
};