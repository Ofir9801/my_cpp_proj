#include "Game.h"
#include <conio.h> 
#include <windows.h>
#include "Utils.h"
#include "Rooms.h"
#include "Player.h"
#include "Screen.h"
#include <cctype> //  for tolower, isdigit


Game::Game() :
	player1(point(1, 4, objSigns::PLAYER1), keys1, board),
	player2(point(75, 4, objSigns::PLAYER2), keys2, board) {
}

void Game::run() {
	hideCursor();
	int gamecycle = 0;
	bool started = true;
	showMenu(started);
	if (!started) {
		cls();
		board.showMessage("Exiting game. Goodbye!");
		Sleep(1000);
		cls();
		gotoxy(0, 0);
		return;
	}
	char winningDoorId = char(board.getCurrentRoom() - 1 + '0');
	board.drawMap();
	player1.draw();
	player2.draw();
	bool exitGame = started;
	while (exitGame) {
		updateSwitches();
		board.showPlayerInfo(player1);
		board.showPlayerInfo(player2);
		board.refreshSpringsDisplay(player1.getPosition(), player2.getPosition());
		board.clearMessegeArea(gamecycle);

		player1.move();
		player2.move();
		Sleep(130);
		
		if (_kbhit()) {
			int key = _getch();

			if (key == ESC) {  //change to const ESC	
				board.showMessage("Game Paused. Press ESC again to continue or H to exit.");
				key = _getch();
				if (std::tolower((unsigned char)key) == std::tolower('h')) {
					cls();
					board.showMessage("Exiting game. Goodbye!");
					Sleep(1000);
					cls();
					exitGame = false;
				}
				else {
					gotoxy(0, 24);
					board.drawMap();
				}
			}
			else if (isSpecialKey(key)) {
				char a = _getch(); //ignore special keys like arrows
			}

			else {
				player1.handleKeyPressed((char)key);
				player2.handleKeyPressed((char)key);
			}
		}
		player1.draw();
		player2.draw();
		gamecycle++;

		if (player1.hasFinished() && player2.hasFinished()) {
			size_t index = board.getCurrentRoom();
			if (index < VICTORY) {
				changeRoom(++index);
				winningDoorId = (char)('0' + (board.getCurrentRoom() - 1));
			}
			else {
				board.showMessage("Press Any key to finish the game");
				_getch();
				exitGame = false;
				cls();
			}
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
				changeRoom(ROOM2);
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

void Game::changeRoom(int roomNumber)
{
	board.loadMap(roomNumber);
	board.drawMap();
	if (roomNumber != MENU && roomNumber != INSTRUCTIONS && roomNumber != VICTORY) {
	player1.reset(point(1, 4, objSigns::PLAYER1));
	player2.reset(point(75, 4, objSigns::PLAYER2));
		player1.draw();
		player2.draw();
	}
}

void Game::updateSwitches() {
	for (auto& s : board.switches) {
		bool isPressed = s.isAt(player1.getPosition()) || s.isAt(player2.getPosition());
		s.update(isPressed);
		}
}