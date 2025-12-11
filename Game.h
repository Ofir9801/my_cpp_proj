#pragma once
#include "Screen.h"
#include "Player.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Door.h"
#include "Riddle.h"

class Game{
	Screen board;
	Player player1;
	Player player2;

public:
	Game(); //default constructor to initialize the Game components	
	void run(); //function to run the main Game loop
	void showMenu(bool &started);//function to show the Game menu
	void changeRoom(int roomNumber);
private:
	void updateSwitches();
	bool isSpecialKey(int key) { return key == 0 || key == 224; } //when keyboard presses special keys like arrows
	void SetColorfullGame();
	bool isGameOver() { return !player1.isAlive() && !player2.isAlive(); }
	void LoadRiddles();
	Riddle ReadRiddleFromFile(string FileName, int RiddleIndex, bool& error);
};

