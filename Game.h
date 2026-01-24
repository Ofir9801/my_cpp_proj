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
	virtual void onGameEvent(const Event& e);
	virtual bool getInput(char& c, size_t iteration);
	virtual void drawMap();
	size_t getIteration() const { return gameCycle; }
	virtual bool isGameSilent() const { return false; }
private:
	bool isSpecialKey(int key) const { return key == 0 || key == 224; } //when keyboard presses special keys like arrows
	bool isGameOver() const { return board.getLives() <= 0 || !board.gameState; }
	virtual void handlePause(bool& exitGame);
	virtual void handleGameOver(bool& exitGame);
	void handleLevelCompletion();
protected:
	Screen board;
	Player player1;
	Player player2;
	size_t gameCycle = 0;
	virtual void drawPlayer();
	virtual void wait(int ms);
	virtual void showMenu(bool& started);//function to show the Game menu
	virtual void changeRoom(int room);
	virtual void performRestart();
	virtual void PerformGoToMenu(bool& exitGame);
	void reportResultError(const std::string& message, size_t iteration);
	void SetColorfullGame();
	virtual bool isSaveLoadAllowed() const { return true; }
};

