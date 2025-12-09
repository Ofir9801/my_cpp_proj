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

	char input = _getch();

	if (input == answer) {
		map.drawMap();
		map.showMessage("Correct! +50 Points.");//random score for this moment
		player.increaseScore(50);
		map.setChar(position, ' ');
		return true;
	}
	else {
		map.drawMap();
		map.showMessage("WRONG! -1 Life.");
		player.decreaseLife();
		return false;
	}
}