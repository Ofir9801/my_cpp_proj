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
	bool started = true;

	showMenu(started);
	
	if (!started) {	return;	}
	board.resetStats();
	drawMap();
	drawPlayer();
	/*board.drawMap();
	player1.draw();
	player2.draw();*/

	bool exitGame = true;
	bool firstMessage = true;

	while (exitGame) {
		if (firstMessage) {
			board.showMessage("Welcome to the Adventure Game!");
			Sleep(1500);
			board.clearMessegeArea(true);
			firstMessage = false;
		}

		if (board.currentRoom == roomIndex::VICTORY) {
			board.showMessage("Press Any key to finish the game");
			char c;
			getInput(c, gameCycle);
			exitGame = false;
			continue;
		}

		Point p1Prev = player1.getPosition();
		Point p2Prev = player2.getPosition();

		updateSwitches();
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
		//check that
	/*	player1.draw();
		player2.draw();*/
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
			handleGameOver(exitGame, gameCycle);
			if (!exitGame) { return; }
			continue;
		}
		
		handleLevelCompletion();
		gameCycle++;
	}
	cls();
}

void Game::showMenu(bool& started){
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
			case '8':
				changeRoom(roomIndex::INSTRUCTIONS);
				board.showInstructionBinds();
				a = _getch();
				changeRoom(roomIndex::MENU);
				break;
			case '9':
				started = false;
				inMenu = false;
				break;
			//case '3'
				//function to read save data
			}
		}
	}
}

void Game::changeRoom(roomIndex room){
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

bool Game::getInput(char& c, size_t iteration){
	if (_kbhit()) {
		c = _getch();
		return true;
	}
	return false;
}

void Game::updateSwitches() {
	for (auto& s : board.switches) {
		bool isPressed = s.isAt(player1.getPosition()) || s.isAt(player2.getPosition());
		s.update(isPressed);
		}
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
	board.showMessage("PAUSED: ESC-Continue, H-Menu, R-Restart");

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
		//choice == 's'

	}
}

void Game::handleGameOver(bool& exitGame,size_t& iterarion)
{
	cls();
	std::cout << board.getFinalMessage() << std::endl;
	std::cout <<"Game Over! you lost!"<<std::endl;
	Sleep(1000);
	std::cout << "Press 'R' to Restart,'H' to go to Main Menu, ESC to exit the game";
	onGameEvent(Event(gameCycle, EventType::GAME_OVER, ' ', "GameEnded. Score: " + std::to_string(board.getScore())));
	while (true) {
		char choice;
		if (getInput(choice,gameCycle)) {
			choice = std::tolower(choice);
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
	player1.draw();
	player2.draw();
}

void Game::wait(int ms) {
	Sleep(ms);
}





