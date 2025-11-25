#include "screen.h"
#include <iostream>
#include <windows.h>
#include "utils.h"
#include "objSigns.h"

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
screen::screen()
{
	memset(map, ' ', sizeof(map)); //initialize the map with spaces
	for (int i = 0; i < MAX_Y; i++) {
		map[i][MAX_X] = '\0'; //null-terminate each row
	}
}
void screen::loadMap(const char* arr[])
{
	for (int i = 0; i < MAX_Y; i++) {
		strncpy_s(map[i], arr[i],MAX_X);
		map[i][MAX_X] = '\0'; //null-terminate each row
	}

}
void screen::draw(){
	cls(); //clear the console
	for (int i = 0; i < MAX_Y; i++) {
		gotoxy(0, i);
		cout << map[i];
	}
}

bool screen::isWall(const point& p) const{
	char c = getCharAt(p);
	return c == '-' || c == '|';
}

void screen::showPlayerInfo(player p) {
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











