#include "Game.h"
#include <conio.h> 
#include <windows.h>
#include "utils.h"
#include "objSigns.h"
#include "Rooms.h"
#include "player.h"
const char keys1[] = "wdsaeq";
const char keys2[] = "ilkjou";

Game::Game() :
	player1(point(1, 4, objSigns::PLAYER1), keys1, board),
	player2(point(75, 4, objSigns::PLAYER2), keys2, board)
{
	board.loadMap(room1);
}

void Game::run() {
	hideCursor();
	showMenu();
	board.draw();
	bool exitGame = false;
	while (!exitGame) {
		player1.move();
		player2.move();

		board.showPlayerInfo(player1);
		board.showPlayerInfo(player2);

		Sleep(100);
		if (_kbhit()) {
			char key = _getch();

			if (key == 27) { 
				gotoxy(0, 24);
				std::cout << "Game Paused. Press ESC again to continue or 9 to exit.";
				key = _getch();
				if (key == '9') exitGame = true;
				else {
					gotoxy(0, 24);
					std::cout << "--------------------------------------------------------";
				}
			}
			else {
				player1.handleKeyPressed(key);
				player2.handleKeyPressed(key);
			}
		}
	}
}

void Game::showMenu(){
	board.loadMap(menu);
	board.draw();
	bool inMenu = true;
	char a;
	while (inMenu) {
		if (_kbhit()) {
			char key = _getch();
			switch (key) {
			case '1':
				board.loadMap(room1);
				inMenu = false;
				break;
			case '8':
				board.loadMap(instructions);
				board.draw();
				a = _getch();
				board.loadMap(menu);
				board.draw();
				break;
			case '9':
				inMenu = false;
				break;
			}
		}
	}
}
