#pragma once
#include "screen.h"
#include "player.h"


class Game{
	screen board;
	player player1;
	player player2;
public:
	Game(); //default constructor to initialize the game components	
	void run(); //function to run the main game loop
	void showMenu(bool started);//function to show the game menu
};

