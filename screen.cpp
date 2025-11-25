#include "Screen.h"
#include <iostream>
#include <windows.h>
#include "Utils.h"
#include "objSigns.h"
#include "Rooms.h"
#include <cctype> //  for tolower, isdigit

using std::cout;
using std::endl;

enum INFO_SLOTS {
	PLAYER1_SIGN_START_X = 11,
	PLAYER2_SIGN_START_X = 70,
	PLAYER_SIGN_Y = 0,
	PLAYER1_INV_START_X = 11,
	PLAYER2_INV_START_X = 70,
	PLAYER_INV_Y = 1
};
Screen::Screen() 
{
	memset(map, ' ', sizeof(map)); //initialize the map with spaces
	for (int i = 0; i < MAX_Y; i++) {
		map[i][MAX_X] = '\0'; //null-terminate each row
	}
	initaializeRoomsArray();

}
void Screen::loadMap(const char* arr[])
{
	for (int i = 0; i < MAX_Y; i++) {
		strncpy_s(map[i], arr[i],MAX_X);
		map[i][MAX_X] = '\0'; //null-terminate each row
	}

}
void Screen::draw(){
	cls(); //clear the console
	for (int i = 0; i < MAX_Y; i++) {
		gotoxy(0, i);
		cout << map[i];
	}
}

bool Screen::isWall(const point& p) const{
	char c = getCharAt(p);
	if (isdigit(c)) {
		//function to open door and change room
		return false;
	}
	return c == '-' || c == '|' || c == '#';
}

void Screen::showPlayerInfo(Player p) {
	char playerChar = p.getChar();
	switch (playerChar) {
	case objSigns::PLAYER1:
			gotoxy(PLAYER1_SIGN_START_X, PLAYER_SIGN_Y);
			cout << playerChar << std::flush;
			gotoxy(PLAYER1_INV_START_X, PLAYER_INV_Y);
			cout << p.getInventory() << std::flush;
		break;
		case objSigns::PLAYER2:
			gotoxy(PLAYER2_SIGN_START_X, PLAYER_SIGN_Y);
			cout << playerChar << std::flush;
			gotoxy(PLAYER2_INV_START_X, PLAYER_INV_Y);
			cout << p.getInventory()<< std::flush;
			break;
	}
}
void Screen::setChar(const point& p, char c) {
	if (p.getX() < 0 || p.getX() >= MAX_X || p.getY() < 0 || p.getY() >= MAX_Y)
		return;
	map[p.getY()][p.getX()] = c;
	gotoxy(p.getX(), p.getY());
	cout << c;
}

void Screen::initaializeRoomsArray() {
	Rooms[0] = room1;
	Rooms[1] = nullptr;
	Rooms[2] = nullptr;
}












