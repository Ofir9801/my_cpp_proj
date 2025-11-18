#include <iostream>
#include "Game.h"
#include <conio.h>
#include "utils.h"
#include "player.h"
#include <windows.h>
#include "objSigns.h"
#include "Rooms.h"

using std::cout;
int main() {
	const char keys1[NUM_KEYS + 1] = "wdsaeq";
	const char keys2[NUM_KEYS + 1] = "ilkjou";
	constexpr char ESC = 27;
	screen board;
	board.loadMap(menu);
	player players[2] = { player(point(1, 4, objSigns::PLAYER1), keys1, board),player(point(75, 4, objSigns::PLAYER2), keys2, board) };
	board.draw();
	hideCursor();
	bool won = false;
	Game game;
	game.run();
	/*
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
			}
			board.showPlayerInfo(p);
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
	cls();*/
}