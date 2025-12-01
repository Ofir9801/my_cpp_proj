#include "Game.h"
#include <conio.h> 
#include <windows.h>
#include "Utils.h"
#include "objSigns.h"
#include "Rooms.h"
#include "Player.h"
#include "Screen.h"
#include <cctype> //  for tolower, isdigit


Game::Game() :
	player1(point(1, 4, objSigns::PLAYER1), keys1, board),
	player2(point(75, 4, objSigns::PLAYER2), keys2, board) {
	loadItems();
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
	board.drawMap();
	player1.draw();
	player2.draw();
	bool exitGame = started;
	while (exitGame) {
		player1.move();
		player2.move();

		board.showPlayerInfo(player1);
		board.showPlayerInfo(player2);
		board.refreshSpringsDisplay(player1.getPosition(), player2.getPosition());
		Sleep(130);
		board.clearMessegeArea(gamecycle);
		if (_kbhit()) {
			char key = (char)_getch();

			if (key == ESC) {  //change to const ESC	
				board.showMessage("Game Paused. Press ESC again to continue or H to exit.");
				key = (char)_getch();
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
			else {
				player1.handleKeyPressed(key);
				player2.handleKeyPressed(key);
			}
		}
		if (board.getSuccessfulMove()) {
			board.setSuccessfulMove(false);
			size_t index = board.getCurrentRoom();
			if (index < NUM_ROOMS - 1)//to see if there is a next room
				changeRoom(++index);
		}
		updateSwitches();
		gamecycle++;
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
void Game::loadItems() {//enter the switches from the board to the vector
	switches.clear();
	obstacles.clear();
	for (int y = 0; y < BOARD_DIMENSION::MAX_Y; y++) {
		for (int x = 0; x < BOARD_DIMENSION::MAX_X; x++) {
			char c = board.getCharAt(point(x, y));
			if (c == '\\') {
				switches.push_back(Switch(x, y, board, false));
			}
			else if (c == '/') {
				switches.push_back(Switch(x, y, board, true));
			}
			else if (c == '*'){
				obstacles.push_back(Obstacle(x, y, board, 1));
			}
			else if(isdigit((unsigned char)c)){
				doors.push_back(Door(x, y, c, board));
			}
		}
	}
	autoLinkSwitchesAndDoors();
}
void Game::changeRoom(int roomNumber)
{
	board.loadMap(roomNumber);
	loadItems();
	player1.reset(point(1, 4, objSigns::PLAYER1));
	player2.reset(point(75, 4, objSigns::PLAYER2));
	board.drawMap();
	if (roomNumber != MENU && roomNumber != INSTRUCTIONS) {
		player1.draw();
		player2.draw();
	}
}
void Game::autoLinkSwitchesAndDoors() {
	int levelNum = board.getCurrentRoom() - 1;
	char currentDoorId = '1';
	char exitDoor = char(levelNum);
	for(auto& s : switches) {
		s.setTargetDoorId(currentDoorId);
		currentDoorId++;
	}
}

void Game::updateSwitches() {
	for (auto& s : switches) {
		bool isPressed = s.isAt(player1.getPosition()) || s.isAt(player2.getPosition());
		if (s.update(isPressed)) {
			int targetID = s.getTargetDoorId();
			for (auto& d : doors) {
				if (d.getId() == targetID) {
					s.getIsOn() ? d.open() : d.close();
				}
			}
		}
	}
}

