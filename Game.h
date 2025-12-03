#pragma once
#include "Screen.h"
#include "Player.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Door.h"


class Game{
	Screen board;
	Player player1;
	Player player2;

public:
	Game(); //default constructor to initialize the Game components	
	void run(); //function to run the main Game loop
	void showMenu(bool &started);//function to show the Game menu
	void loadItems();
	void changeRoom(int roomNumber);
private:
	void autoLinkSwitchesAndDoors();
	void updateSwitches();
	bool isSpecialKey(int key) { return key == 0 || key == 224; } //when keyboard presses special keys like arrows
	void checkPlayerExit(Player& p);
};

