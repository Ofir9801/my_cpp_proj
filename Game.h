#pragma once
#include "Screen.h"
#include "Player.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Door.h"
#include "Riddle.h"
#include "Steps.h"
#include "Results.h"
#include "Event.h"

class Game {
	bool messageShown = false;
public:
	Game(); //default constructor to initialize the Game components	
	virtual ~Game(){}
	void run(); //function to run the main Game loop
private:
	void updateSwitches();
	bool isSpecialKey(int key) const { return key == 0 || key == 224; } //when keyboard presses special keys like arrows
	void SetColorfullGame();
	virtual bool isGameOver(size_t iteration) const { return board.getLives() <= 0 || !board.gameState; }
	bool isPlayableRoom(roomIndex room) { return !(room == roomIndex::MENU || room == roomIndex::INSTRUCTIONS || room == roomIndex::VICTORY); }
	bool isPlayableRoom(int room) { return isPlayableRoom(static_cast<roomIndex>(room)); }
	void performRestart(size_t& gameCycle);
	void PerformGoToMenu(bool& exitGame, size_t& gameCycle);
	void handlePause(bool& exitGame, size_t& gameCycle);
	virtual void handleGameOver(bool& exitGame, size_t& gameCycle);
	void handleLevelCompletion();
	size_t getIteration() const { return gameCycle; }
	virtual void onGameEvent(const Event& e);
protected:
	Screen board;
	Player player1;
	Player player2;
	size_t gameCycle = 0;
	virtual bool getInput(char& c, size_t iteration) = 0;
	virtual void draw();
	virtual void wait(int ms);
	virtual void showMenu(bool& started);//function to show the Game menu
	virtual void changeRoom(roomIndex room);
	bool ImportantkeyPressed(char c);
	void reportResultError(const std::string& message, size_t iteration);
};

