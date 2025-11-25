#include "Game.h"
#include <conio.h> 
#include <windows.h>
#include "utils.h"
#include "objSigns.h"
#include "Rooms.h"
#include "player.h"
const char keys1[] = "wdsaeq";
const char keys2[] = "ilkjou";
const char ESC = 27;
Game::Game() :
	player1(point(1, 4, objSigns::PLAYER1), keys1, board),
	player2(point(75, 4, objSigns::PLAYER2), keys2, board) {}

void Game::run() {
	hideCursor();
	bool started = true;
	showMenu(started);
	board.draw();
	bool exitGame = started;
	while (exitGame) {
		player1.move();
		player2.move();

		board.showPlayerInfo(player1);
		board.showPlayerInfo(player2);

		Sleep(100);
		if (_kbhit()) {
			char key = (char)_getch();

			if (key == ESC) {  //change to const ESC	
				board.showMessage("Game Paused. Press ESC again to continue or H to exit.");
				key = (char)_getch();
				if (std::tolower(key) == std::tolower ('h')) exitGame = false;
				else {
					gotoxy(0, 24);
					board.showMessage("--------------------------------------------------------");
				}
			}
			else {
				player1.handleKeyPressed(key);
				player2.handleKeyPressed(key);
			}
		}
	}
}

void Game::showMenu(bool& started){
	board.loadMap(menu);
	board.draw();
	bool inMenu = true;
	char a;
	while (inMenu) {
		if (_kbhit()) {
			char key = (char)_getch();
			switch (key) {
			case '1':
				board.loadMap(room1);
				inMenu = false;
				break;
			case '8':
				board.loadMap(instructions);
				board.draw();
				a = (char)_getch();
				board.loadMap(menu);
				board.draw();
				break;
			case '9':
				started = false;
				inMenu = false;
				break;
			}
		}
	}
}




