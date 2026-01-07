#include "Riddle.h"
#include <iostream>
#include <conio.h>
#include "Utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <fstream>

bool Riddle::engage(Player& player, std::string& answer){
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
				answer = options[choiceIndex];
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

bool Riddle::engageVaultRiddle(std::string& str) {
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