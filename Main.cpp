#include <iostream>
#include "Game.h"
#include <conio.h>
#include "utils.h"
#include "player.h"
#include <windows.h>

using std::cout;
int main() {
	const char keys1[NUM_KEYS + 1] = "wdxase";
	const char keys2[NUM_KEYS + 1] = "ilmjko";
	constexpr char ESC = 27;
	screen board;
	player players[2] = { player(point(1, 1, '@'), keys1, board),player(point(1, 5, '&'), keys2, board) };
	board.draw();
	bool won = false;
	while (!won) {
		for (auto& p : players) {
			p.move();
			/*
			if (p.hasWon()) {
				Sleep(20);

				cls();
				std::cout << p.getChar() << " won!" << std::endl;
				(void)_getch();
				won = true;
				break;
			}*/
		}
		Sleep(100);
		if (_kbhit()) {
			char key = _getch();
			if (key == ESC) {
				// pause
				key = _getch();
				if (key == 'h' || key == 'H') {
					break;
				}
			}
			else {
				for (auto& p : players) {
					p.handleKeyPressed(key);
				}
			}
		}
	}
	cls();
}