#include "Riddle.h"
#include "Screen.h"
#include "Player.h"
#include <iostream>
#include <conio.h>
#include "Utils.h"

bool Riddle::engage(Screen& map, Player& player) const {
	cls();
	gotoxy(0, 10);
	std::cout << "RIDDLE TIME!" << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << question << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "Press 1-4 to answer..." << std::endl;
	for(int i = 1; i <= options.size(); ++i) {
		std::cout << i << ". " << options[i - 1] << std::endl;
	}
	while (true) {//waiting for a proper answer from user
		if (_kbhit()) {
			char key = _getch();
			if (key >= '1' && key < '1' + options.size()) {
				int choiceIndex = key - '1';
				if (choiceIndex == correctIndex) {
					map.showMessage("Correct! The path is clear. +100 points!");
					player.increaseScore(100);
					return true; 
				}
				else {
					map.showMessage("WRONG! You lost 50 points.");
					player.increaseScore(-50);
					return false;
				}
			}
		}
	}
}