#include "Riddle.h"
#include <iostream>
#include <conio.h>
#include "Utils.h"
#include <algorithm>
#include <fstream>
#include "Game.h"

bool Riddle::engage(Player& player, std::string& answer, Game* game){
	if (!game->isGameSilent()) {
		cls();
		gotoxy(10, 10);
		std::cout << "RIDDLE TIME!" << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << question << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << "Press 1-4 to answer..." << std::endl;

		for (size_t i = 1; i <= options.size(); ++i) {
			std::cout << i << ". " << options[i - 1] << std::endl;
		}
	}
	char key = 0;
	while (true) {//waiting for a proper answer from user
		if (game->getInput(key, game->getIteration())) {
			if (key >= '1' && key < '1' + options.size()) {
				int choiceIndex = key - '1';
				answer = options[choiceIndex];
				if (choiceIndex == correctIndex) {
					player.increaseScore(SUCCESS_SCORE);
					solved = true;
					return true; 
				}
				else {
					player.increaseScore(-WRONG_ANSWER);
					return false;
				}
			}
		}
	}
}

bool Riddle::engageVaultRiddle(std::string& str, Game* game) {
	if (!game->isGameSilent()) {
		cls();
		gotoxy(RIDDLE_X, RIDDLE_Y);
		std::cout << "RIDDLE TIME!" << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << question << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << "Press 1-4 to answer..." << std::endl;
	}
	string answer = "";
	char key = 0;
	while (true) {
		if (game->getInput(key, game->getIteration())) {
			if (key == ENTER) {
				if(!answer.empty()) {
					break; //enter pressed
				}
			}
			else if (key == BACKSPACE) { 
				if (!answer.empty()) {
					answer.pop_back();
					if (!game->isGameSilent()) {
						std::cout << "\b \b"; //handle backspace
					}
					
				}
			}
			else if(isdigit(key)) {
				answer += key;
				if (!game->isGameSilent()) {
					std::cout << key; //display the typed character
				}
			}
		}
	}
	str = answer;
	if (answer == correctAnswer) {
		solved = true;
		return true;
	}
	return false;
}

void Riddle::save(std::ofstream& file) const {
	file << question << "\n";
	file << correctIndex << "\n";
	file << (solved ? 1 : 0) << " ";
	saveVector(file, options);
}

void Riddle::load(std::ifstream& file) {
	file.ignore();
	std::getline(file,question);
	if (!question.empty() && question.back() == '\r') question.pop_back();

	file >> correctIndex;
	bool flag;
	file >> flag;
	solved = (flag == 1);

	size_t size;
	file >> size;
	file.ignore();
	options.clear();

	for (size_t i = 0; i < size; i++) {
		std::string temp;
		std::getline(file, temp);
		options.push_back(temp);
	}
	correctAnswer = options[correctIndex];
}