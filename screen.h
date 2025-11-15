#pragma once
//would contain screen related functions for the game map
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;
void gotoxy(int x, int y); //function to move the console cursor to a specific position
class screen
{
	char map[SCREEN_HEIGHT][SCREEN_WIDTH];
	//screen rectangle for 80x25 characters for the static objects
public:
	screen(); //default constructor to initialize the map with no objects
	void draw(); //function to draw the map to the console
	char getCharAt(int x, int y) {
		return map[y][x];
	} //function to get the character at a specific position
};

