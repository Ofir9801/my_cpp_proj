#include "screen.h"
#include <iostream>
#include <windows.h>
#include "utils.h"
using std::cout;
using std::endl;

screen::screen()
{
	//initialize the map with general walls
	for (int i = 0; i < MAX_Y; i++) {
			for (int j = 0; j < MAX_X; j++) {
				if (i == 0 || i == MAX_Y - 1) 
					map[i][j] = '|'; //walls at the borders
				else if (j == 0 || j == MAX_Y - 1)
					map[i][j] = '-'; //walls at the borders
				else 
					map[i][j] = ' '; //empty space inside
					}
	}
}
void screen::draw(){
	utils::cls(); //clear the console
	gotoxy(0, 0);
	for (size_t i = 0; i < MAX_Y - 1; ++i) {
		cout << map[i] << endl;
	}
	cout << map[MAX_Y - 1];
	cout.flush();
	//draw the map to the console
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			gotoxy(x, y);
			cout << map[y][x];
		}
		cout << endl;
	}
}





