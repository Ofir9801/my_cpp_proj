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
//	loadItems();
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
		
		player1.move();
		player2.move();
		updateSwitches();
		checkPlayerExit(player1);
		checkPlayerExit(player2);
		if (player1.hasFinished() && player2.hasFinished()) {
			board.setSuccessfulMove(false);
			size_t index = board.getCurrentRoom();
			if (index < NUM_ROOMS - 1) {
				changeRoom(++index);
				winningDoorId = (char)('0' + (board.getCurrentRoom() - 1));
			}
			else {
				exitGame = false;
			}
		}
		if (!player1.hasFinished()) player1.draw();
		if (!player2.hasFinished()) player2.draw();
		board.showPlayerInfo(player1);
		board.showPlayerInfo(player2);
		board.refreshSpringsDisplay(player1.getPosition(), player2.getPosition());
		Sleep(130);
		board.clearMessegeArea(gamecycle);
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
			else if(isSpecialKey(key)){
				char a = _getch(); //ignore special keys like arrows
			}
			
			else{
				player1.handleKeyPressed((char)key);
				player2.handleKeyPressed((char)key);
			}
		}
		player1.draw();
		player2.draw();
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
/*
void Game::loadItems() {//enter the items from the board to the vector
	switches.clear();
	obstacles.clear();
	doors.clear();
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
}*/
void Game::changeRoom(int roomNumber)
{
	board.loadMap(roomNumber);
	
	player1.reset(point(1, 4, objSigns::PLAYER1));
	player2.reset(point(75, 4, objSigns::PLAYER2));
	board.drawMap();
	if (roomNumber != MENU && roomNumber != INSTRUCTIONS) {
		player1.draw();
		player2.draw();
	}
}
/*
void Game::autoLinkSwitchesAndDoors() {
	int levelNum = board.getCurrentRoom() - 1;
	char currentDoorId = '1';
	char exitDoor = char(levelNum);
	for(auto& s : switches) {
		s.setTargetDoorId(currentDoorId);
		currentDoorId++;
	}
}*/

void Game::updateSwitches() {
	for (auto& s : board.switches) {
		bool isPressed = s.isAt(player1.getPosition()) || s.isAt(player2.getPosition());
		if (s.update(isPressed)) {
			int targetID = s.getTargetDoorId();
			for (auto& d : board.doors) {
				if (d.getId() == targetID) {
					s.getIsOn() ? d.open() : d.close();
				}
			}
		}
	}
}
bool Screen::isDoorOpen(int door_id)
{
	for (const auto& d : doors) {
		if (d.getId() == door_id) {
			return d.getIsOpen();
		}
	}
}
void Screen::openDoor(int door_id)
{
	for (auto& d : doors) {
		if (d.getId() == door_id) {
			d.open();
		}
	}
}
void Game::checkPlayerExit(Player& p) {
	if (p.hasFinished()) return;
	if (board.isOnOpenDoor(p.getPosition())) {
		char winningID = (char)('0' + (board.getCurrentRoom() - 1));
		for (const auto& d : board.doors) {
			if (d.isAt(p.getPosition())) {
				if (d.getId() == winningID) {
					p.reachedExit();
					board.showMessage("Waiting for partner...");
				}
				else {
					board.showMessage("Wrong door! Dead end.");
				}
				return;
			}
		}
	}
}