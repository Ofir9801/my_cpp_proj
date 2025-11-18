#include "Game.h"
const char keys1[NUM_KEYS + 1] = "wdxase";
const char keys2[NUM_KEYS + 1] = "ilmjko";
Game::Game() :
	player1(point(1, 1, '@'), keys1, board),
	player2(point(1, 5, '&'), keys2, board)
	//initialize players with their positions and control keys
{
	hideCursor();
	cls();
	board.draw();
}

void Game::run(){
}

void Game::showMenu(){

}
