#include "Game.h"
#include <conio.h> 
#include <windows.h>
#include "Utils.h"
#include "objSigns.h"
#include "Rooms.h"
#include "Player.h"
#include "Screen.h"
#include <cctype> //  for tolower, isdigit

const char keys1[] = "wdsaeq";
const char keys2[] = "ilkjou";
const char ESC = 27;


Game::Game() :
	player1(point(1, 4, objSigns::PLAYER1), keys1, board),
	player2(point(75, 4, objSigns::PLAYER2), keys2, board) {
	loadSwitches();
}

void Game::run() {
	hideCursor();
	bool started = true;
	showMenu(started);
	board.drawMap();
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
				if (std::tolower((unsigned char)key) == std::tolower ('h')) exitGame = false;
				else {
					gotoxy(0, 24);
					board.drawMap();
				}
			}
			else {
				player1.handleKeyPressed(key);
				player2.handleKeyPressed(key);
			}
		}
		if (board.getSuccessfulMove()) {
			size_t index = board.getCurrentRoom();
			changeRoom(index++);
		}

	}
}

void Game::showMenu(bool& started){
	changeRoom(MENU);
	bool inMenu = true;
	char a;
	while (inMenu) {
		if (_kbhit()) {
			char key = (char)_getch();
			switch (key) {
			case '1':
				changeRoom(ROOM1);
				inMenu = false;
				break;
			case '8':
				changeRoom(INSTRUCTIONS);
				board.showKeyBinds(keys1,keys2);
				a = (char)_getch();
				changeRoom(MENU);
				break;
			case '9':
				started = false;
				inMenu = false;
				break;
			}
		}
	}
}
void Game::loadSwitches() {//enter the switches from the board to the vector
	switches.clear();
	for (int y = 0; y < Screen::MAX_Y; y++) {
		for (int x = 0; x < Screen::MAX_X; x++) {
			char c = board.getCharAt(point(x, y));
			if (c == '\\') {
				switches.push_back(Switch(x, y, board, false));
			}
			else if (c == '/') {
				switches.push_back(Switch(x, y, board, true));
			}
		}
	}
}

void Game::changeRoom(int roomNumber)
{
	board.loadMap(roomNumber);
	board.drawMap();
}






