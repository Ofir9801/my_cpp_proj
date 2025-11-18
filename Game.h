#pragma once
#include "screen.h"
#include "player.h"


class Game{
	screen board;
public:
	Game(); //default constructor to initialize the game components
	void run(); //function to run the main game loop
	void showMenu();//function to show the game menu
};

