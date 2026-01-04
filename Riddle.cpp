#include "Riddle.h"
#include <iostream>
#include <conio.h>
#include "Utils.h"
#include <algorithm>


bool Riddle::engage(Player& player){
	cls();
	gotoxy(10, 10);
	std::cout << "RIDDLE TIME!" << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << question << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "Press 1-4 to answer..." << std::endl;
	for(size_t i = 1; i <= options.size(); ++i) {
		std::cout << i << ". " << options[i - 1] << std::endl;
	}
	while (true) {//waiting for a proper answer from user
		if (_kbhit()) {
			char key = static_cast<char>(_getch());
			if (key >= '1' && key < '1' + options.size()) {
				int choiceIndex = key - '1';
				if (choiceIndex == correctIndex) {
					player.increaseScore(100);
					solved = true;
					return true; 
				}
				else {
					player.increaseScore(-50);
					return false;
				}
			}
		}
	}
}

bool Riddle::engageVaultRiddle() {
	cls();
	gotoxy(10, 10);
	std::cout << "RIDDLE TIME!" << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << question << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "Enter the numbers sorted by room number" << std::endl;

	string answer;
	bool validInput = false;
	while (!validInput) {
		std::cin >> answer;
		validInput = !answer.empty() && std::all_of(answer.begin(), answer.end(), ::isdigit);
	}
	if (answer == correctAnswer) {
		solved = true;
		return true;
	}
	return false;
}