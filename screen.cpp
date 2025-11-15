#include "screen.h"
#include <iostream>
#include <windows.h>
using std::cout, std::endl;

screen::screen()
{
	//initialize the map with general walls
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
			for (int j = 0; j < SCREEN_WIDTH; j++) {
				if (i == 0 || i == SCREEN_HEIGHT - 1) 
					map[i][j] = '|'; //walls at the borders
				else if (j == 0 || j == SCREEN_WIDTH - 1)
					map[i][j] = '-'; //walls at the borders
				else 
					map[i][j] = ' '; //empty space inside
					}
	}
}
void screen::draw()
{
	//draw the map to the console
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			gotoxy(x, y);
			cout << map[y][x];
		}
		cout << endl;
	}
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << "kzjdbkjxzdc";
}



