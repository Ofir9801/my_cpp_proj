#pragma once
#include <conio.h> 
#include <windows.h>
#include "Game.h"
#include "Utils.h"
#include "Player.h"
#include "Screen.h"
#include <random>
#include <algorithm>
#include <cctype> //  for tolower, isdigit

Game::Game() :
	board(0),
	player1(Point(PLAYER_1_START_X, PLAYER_1_START_Y, objSigns::PLAYER1), keys1, board),
	player2(Point(PLAYER_2_START_X, PLAYER_2_START_Y, objSigns::PLAYER2), keys2, board) 
{
	board.setGame(this);
}

void Game::run() {
	hideCursor();
	gameCycle = 0;
	bool exitGame = true;

	showMenu(exitGame);
	board.resetStats();
	drawMap();
	drawPlayer();
	
	
	bool firstMessage = true;

	while (exitGame) {
		if (firstMessage) {
			board.showMessage("Welcome to the Adventure Game!");
			wait(1500);
			board.clearMessegeArea(true);
			firstMessage = false;
		}

		if (board.currentRoom == roomIndex::VICTORY) {
			board.showMessage("Press Any key to finish the game");
			char c;
			while (!getInput(c, gameCycle)) {
				wait(100);
				gameCycle++;
			}
			exitGame = false;
			continue;
		}

		Point p1Prev = player1.getPosition();
		Point p2Prev = player2.getPosition();

		board.handleSwitches(p1Prev, p2Prev);
		board.updateBombs(player1, player2);
		board.showPlayerInfo(player1);
		board.showPlayerInfo(player2);
		board.refreshSpringsDisplay(player1.getPosition(), player2.getPosition());
		board.clearMessegeArea();

		player1.move();
		player2.move();

		if (board.isDark()) {
			board.updateLighting(player1.getPosition(), p1Prev, player1,
								player2.getPosition(), p2Prev, player2);
		}
		wait(100);
		drawPlayer();

		char key;
		if (getInput(key, gameCycle)) {
			if (key == ESC) { 
				handlePause(exitGame);
			}
			else if (isSpecialKey(key)) {
				(void)_getch(); //ignore special keys like arrows
			}
			else {
				player1.handleKeyPressed(key);
				player2.handleKeyPressed(key);
			}
		}

		if(isGameOver()) {
			handleGameOver(exitGame);
			if (!exitGame) { return; }
			continue;
		}
		
		handleLevelCompletion();
		gameCycle++;
	}
	cls();
}

void Game::showMenu(bool& exitGame){

	changeRoom(roomIndex::MENU);
	bool inMenu = true;
	int a;
	while (inMenu) {
		if (_kbhit()) {
			int key = _getch();
			switch (key) {
			case '1':
				changeRoom(roomIndex::ROOM1);
				inMenu = false;
				break;
			case '2':
				SetColorfullGame();
				changeRoom(roomIndex::ROOM1);
				inMenu = false;
				break;
			case '3': {
				if (!isSaveLoadAllowed()) {
					std::string msg = "Loading games is disabled in Recording Mode (-save)";
					const int X_coord = MAX_X / 2 - static_cast<int>(msg.size()) / 2;
					const int y_coord = MAX_Y / 2;
					gotoxy(X_coord, y_coord);
					std::cout << msg << std::endl;
					wait(2000); 
					board.drawMap();  
					break;

				}
				std::string selectedFile = board.selectSaveFile();

				if (selectedFile.empty()) {
					board.drawMap();
					std::string msg = "No saved files in memory";
					const int X_coord = MAX_X / 2 - static_cast<int>(msg.size()) / 2;
					const int y_coord = MAX_Y / 2;
					gotoxy(X_coord, y_coord);
					std::cout << msg << std::endl;
				}
				else if (selectedFile == "ESC") {
					board.drawMap();
				}
				else {
					int room = board.loadGame(selectedFile);
					if (room != roomIndex::INSTRUCTIONS) {
						changeRoom(static_cast<roomIndex>(room));
						inMenu = false;
					}
					else {
						std::string msg = "Error: File corrupted or could not open.";
						const int X_coord = MAX_X / 2 - static_cast<int>(msg.size()) / 2;
						const int y_coord = MAX_Y / 2;
						gotoxy(X_coord, y_coord);
						std::cout << msg << std::endl;
					}
				}
				break;

			}
			case '7':
				if (!isSaveLoadAllowed())
				{
					std::string msg = "Clear memory is disabled in Recording Mode (-save)";
					const int X_coord = MAX_X / 2 - static_cast<int>(msg.size()) / 2;
					const int y_coord = MAX_Y / 2;
					gotoxy(X_coord, y_coord);
					std::cout << msg << std::endl;
					wait(2000);
					board.drawMap();
					break;
				}
				board.cleanSavedGames();
				board.drawMap();
				break;
			case '8':
				changeRoom(roomIndex::INSTRUCTIONS);
				board.showInstructionBinds();
				a = _getch();
				changeRoom(roomIndex::MENU);
				break;
			case '9':
				exitGame = false;
				inMenu = false;
				break;
			}
		}
	}
}

void Game::changeRoom(roomIndex room){
	if (room != roomIndex::MENU) {
		onGameEvent(Event(getIteration(), EventType::ROOM_CHANGE, ' ', "Move to Room " + std::to_string(static_cast<int>(room))));
	}
	int prevRoom = board.getCurrentRoom();
	if (prevRoom < roomIndex::VICTORY) {
		board.saveRoom();
	}
	Point doorPos(PLAYER_1_START_X, PLAYER_1_START_Y);
	int roomNumber = static_cast<int>(room);
	board.loadMap(roomNumber, doorPos);
	if (isPlayableRoom(room)) {
		if (!isPlayableRoom(prevRoom)) {
			player1.reset(Point(PLAYER_1_START_X, PLAYER_1_START_Y, objSigns::PLAYER1));
			player2.reset(Point(PLAYER_2_START_X, PLAYER_2_START_Y, objSigns::PLAYER2));
		}
		else {
				doorPos.setChar(objSigns::PLAYER1);
				player1.reset(doorPos);
				doorPos.setChar(objSigns::PLAYER2);
				player2.reset(doorPos);
			}
		}
	if (room == roomIndex::ROOM3)
		board.showMessage("it is very dark in here, you will need something to light it up");
}

bool Game::ImportantkeyPressed(char c)
{
	return (c == ESC) || player1.ImportantKeyPressed(c)|| player2.ImportantKeyPressed(c);
}
// Base class version: no need of event e
void Game::onGameEvent(const Event& /*e*/) {}

// Base class version: no need of iterarion
bool Game::getInput(char& c, size_t /*iteration*/) {
	if (_kbhit()) {
		c =static_cast<char>(_getch());
		return true;
	}
	return false;
}

// Enable color mode in the game
void Game::SetColorfullGame() {
	board.colorToggle = true;
}

void Game::performRestart(){
	board.resetStats();
	board.clearSavedRooms();
	board.currentRoom = (size_t)roomIndex::MENU;
	board.setGameState(true);
	player1.resetInventory();
	player2.resetInventory();
	changeRoom(roomIndex::ROOM1);
	gameCycle = 0;
}

void Game::PerformGoToMenu(bool& exitGame)
{
	board.clearSavedRooms();
	board.resetStats();
	board.setGameState(true);
	board.colorToggle = false; //reset color mode when going to menu
	bool gameActive = true;
	showMenu(gameActive);
	if (!gameActive) {
		exitGame = false;
	}
	else {
		gameCycle = 0;
	}
}

void Game::handlePause(bool& exitGame)
{
	board.showMessage("PAUSED: ESC-Continue, H-Menu, R-Restart, S-Save game");

	while (true) {
		char choice = static_cast<char>(std::tolower(_getch()));
		if (choice == ESC) {
			board.showMessage(EMPTYLINE);
			board.drawMap();
			break;
		}
		else if (choice == 'r') {
			performRestart();
			break;
		}
		else if (choice == 'h') {
			board.colorToggle = false; //reset color mode when going to menu
			PerformGoToMenu(exitGame);
			break;
		}
		else if (choice == 's') {
			board.saveRoom();
			board.saveGame();
			board.colorToggle = false;
			PerformGoToMenu(exitGame);
			break;
		}

	}
}

void Game::handleGameOver(bool& exitGame)
{
	std::string winMsg = "";
	cls();
	std::cout << board.getFinalMessage() << std::endl;
	std::cout <<"Game Over! you lost!"<<std::endl;
	Sleep(1000);
	std::cout << "Press 'R' to Restart,'H' to go to Main Menu, ESC to exit the game";
	if (board.getCurrentRoom() == roomIndex::VICTORY) {
		winMsg = "The User win the game";
	}
	else {
		winMsg = "The User lost the game";
	}
	onGameEvent(Event(gameCycle, EventType::GAME_OVER, ' ', "Game Ended: " + winMsg + ". The score is : " + std::to_string(board.getScore())));
	while (true) {
		char choice;
		if (getInput(choice,gameCycle)) {
			choice =static_cast<char>(std::tolower(choice));
			if (choice == 'r') {
				performRestart();
				break;
			}
			else if (choice == 'h') {
				PerformGoToMenu(exitGame);
				break;
			}
			else if (choice == ESC) { // ESC -> Exit the game
				exitGame = false;
				break;
			}
		}
	}
}

void Game::handleLevelCompletion() {
	if (!messageShown &&board.getCurrentRoom() == roomIndex::ROOM2)
	{
		if (board.allRiddlesSolved()){
			board.showMessage("All riddles in this room have been solved. you both get extra inventory space!");
			player1.setExtraInventory(true);
			player2.setExtraInventory(true);
			messageShown = true;
		}
	}

	if (!player1.hasFinished() || !player2.hasFinished()) { return; }
	int player1Room = player1.getRoomOpen(); //the room number of the door opened by player 1
	int player2Room = player2.getRoomOpen(); //the room number of the door opened by player 2
	if (player1Room == player2Room) { changeRoom(static_cast<roomIndex>( player1Room)); }//both players chose the same door
	else {
		string msg = "you chose different rooms!choose a room : " + std::to_string(static_cast<int>(player1Room)) + " / " + std::to_string(static_cast<int>(player2Room));
		board.showMessage(msg);
		while (true) {
			if (_kbhit()) {
				size_t key = _getch();
				if (key <= '9' && key >= '0') {
					size_t chosenRoom = key - '0';

					if (chosenRoom == player1Room || chosenRoom == player2Room) {
						changeRoom((roomIndex)chosenRoom);
						break;
					}
					else {
						board.showMessage("Invalid choice. Please choose again between " + std::to_string(static_cast<int>(player1Room)) + "/" + std::to_string(static_cast<int>(player2Room)));
					}
				}
			}
		}
	}
}

void Game::reportResultError(const std::string& message, size_t iteration) {
	system("cls");
	std::cout << "TEST FAILED at Iteration " << iteration << ": " << message << std::endl;
}

void Game::drawMap() {
	board.drawMap();
}
void Game::drawPlayer()
{
	player1.drawToScreen();
	player2.drawToScreen();
}

void Game::wait(int ms) {
	Sleep(ms);
}





