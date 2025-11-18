#include "Game.h"

Game::Game() {
	hideCursor();
	cls();
	board.draw();
	//initialize players with their positions and control keys
	const char keys1[NUM_KEYS + 1] = "wdxas";
	const char keys2[NUM_KEYS + 1] = "ijklmn";
	player player1(point(1, 1, '@'), keys1, board);
	player player2 (point(1, 5, '&'), keys2, board);
}

void Game::run(){
}

void Game::showMenu(){

}
