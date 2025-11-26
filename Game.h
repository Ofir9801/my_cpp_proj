#pragma once
#include "Screen.h"
#include "Player.h"
#include "Switch.h"
#include <vector>

class Game{
	Screen board;
	Player player1;
	Player player2;
	std::vector<Switch> switches;
public:
	Game(); //default constructor to initialize the Game components	
	void run(); //function to run the main Game loop
	void showMenu(bool &started);//function to show the Game menu
	void loadSwitches();
	void changeRoom(int roomNumber);
};

