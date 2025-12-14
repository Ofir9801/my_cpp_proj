#pragma once
#include <conio.h> 
#include <windows.h>
#include "Game.h"
#include "Utils.h"
#include "Player.h"
#include "Screen.h"
#include <fstream>
#include <random>
#include <algorithm>
#include <cctype> //  for tolower, isdigit


Game::Game() :
	player1(Point(1, 4, objSigns::PLAYER1), keys1, board),
	player2(Point(75, 4, objSigns::PLAYER2), keys2, board) {}

void Game::run() {
	hideCursor();
	int gamecycle = 0;
	bool started = true;
	showMenu(started);
	bool firstMessage = true;
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
		if (firstMessage) {
			board.showMessage("Welcome to the Adventure Game! only one door will lead you to the next room.");
			Sleep(2000);
			firstMessage = false;
		}
		Point p1Prev = player1.getPosition();
		Point p2Prev = player2.getPosition();

		updateSwitches();
		LoadRiddles();
		board.updateBombs(player1, player2);
		board.showPlayerInfo(player1);
		board.showPlayerInfo(player2);
		board.refreshSpringsDisplay(player1.getPosition(), player2.getPosition());
		//board.clearMessegeArea(gamecycle);

		player1.move();
		player2.move();

		if (board.isDark()) {
			board.updateLighting(player1.getPosition(), p1Prev, player1,
								player2.getPosition(), p2Prev, player2);
		}
		player1.draw();
		player2.draw();

		Sleep(100);
		
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
				(void)_getch(); //ignore special keys like arrows
			}

			else {
				player1.handleKeyPressed((char)key);
				player2.handleKeyPressed((char)key);
			}
		}
		if(isGameOver()) {
			cls();
			gotoxy(30, 10);
			board.showMessage("Game Over! both of the players has no lives left.");
			exitGame = false;
			continue;
		}
		gamecycle++;

		if (player1.hasFinished() && player2.hasFinished()) {
			size_t index = board.getCurrentRoom();
			if (index < VICTORY) {
				changeRoom(++index);
				winningDoorId = (char)('0' + (board.getCurrentRoom() - 1));
			}
			else {
				board.showMessage("Press Any key to finish the game");
				(void)_getch();
				exitGame = false;
				cls();
			}
		}
	}
	cls();
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
			case '2':
				SetColorfullGame();
				changeRoom(ROOM3);
				inMenu = false;
				break;
			case '8':
				changeRoom(INSTRUCTIONS);
				board.showKeyBinds();
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
	board.drawMap(roomNumber);
	if (roomNumber != MENU && roomNumber != INSTRUCTIONS && roomNumber != VICTORY) {
	player1.reset(Point(1, 4, objSigns::PLAYER1));
	player2.reset(Point(75, 4, objSigns::PLAYER2));
	player1.draw();
	player2.draw();
	}
	if (roomNumber == 3)
		board.showMessage("it is very dark in here, you will need something to light it up");
}

void Game::updateSwitches() {
	for (auto& s : board.switches) {
		bool isPressed = s.isAt(player1.getPosition()) || s.isAt(player2.getPosition());
		s.update(isPressed);
		}
}

void Game::SetColorfullGame() {
	// Enable color mode in the game
	board.colorToggle = true;
}

void Game::LoadRiddles()
{
	for (int i = 0; i < NUMS_OF_RIDDLES; i++) {
		bool error = false;
		Riddle temp = ReadRiddleFromFile(RiddlePathWay, i, error);
		if (error) {
			throw std::runtime_error("Something wrong with the file riddle.txt");
		}
		else {
			board.riddles.push_back(temp);
		}
		
	}
	
}
Riddle Game::ReadRiddleFromFile(string FileName, int RiddleIndex, bool& error) {
	std::ifstream inFile(FileName);
	std::string question;
	std::string correctAnswer;
	std::vector<std::string> options;
	int correctIndex;

	if (!inFile.is_open()) {
		error = true;
	}
	string templine;
	for (int i = 0; i < RiddleIndex; i++) { //skip to the right riddle index
		for (int j = 0; j < 5; j++) { //every riddle is represnt by 5 lines in riddle.txt
			if (!std::getline(inFile, templine)) {
				templine = "";
			}
		}
	}

	for (int i = 0; i < 5; i++) {
		if (!std::getline(inFile, templine)) {
			templine = "";
		}
		if (i == 0) { question = templine; } //first line is the question
		else if (i == 1) { //second line is the right answers
			correctAnswer = templine;
			options.push_back(correctAnswer);
		}
		else { options.push_back(templine); } // other 3 lines is wrong answers
	}

	inFile.close();

	//randomize the possible answers
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(options.begin(), options.end(), g);
	for (int i = 0; i < options.size(); i++) {
		if (options[i] == correctAnswer)
			correctIndex = i;
	}

	return Riddle(question, options, correctIndex);
}
