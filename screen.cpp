#include "Screen.h"
#include <iostream>
#include <windows.h>
#include "Utils.h"
#include "objSigns.h"
#include "Rooms.h"
#include "Player.h"
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
void Screen::loadMap(int roomNumber)
{
	for (int i = 0; i < MAX_Y; i++) {
		strncpy_s(map[i], Rooms[roomNumber][i],MAX_X);
		map[i][MAX_X] = '\0'; //null-terminate each row
	}
	currentRoom = roomNumber;
}
void Screen::drawMap() {
	cls(); //clear the console
	for (int i = 0; i < MAX_Y; i++) {
		gotoxy(0, i);
		cout << map[i];
	}
}

bool Screen::isWall(const point& p) const{
	char c = getCharAt(p);
	return c == '-' || c == '|' || c == '*';
}

void Screen::showPlayerInfo(const Player& p) {
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

void Screen::showKeyBinds(const char* keys1, const char* keys2) const
{
	int const INITIAL_Y = 19;
	int const INITIAL_X1 = 11;
	int const INITIAL_X2 = 40;

	for (int i = 0; i < NUM_KEYS; i++) {
		gotoxy(INITIAL_X1, INITIAL_Y + i);
		cout << (unsigned char)toupper(keys1[i]); //print uppercase
		gotoxy(INITIAL_X2, INITIAL_Y + i);
		cout << (unsigned char)toupper(keys2[i]); //print uppercase
	}
}
void Screen::showMessage(const char* msg){
	gotoxy(MESSAGES_POS::MES_X, MESSAGES_POS::MES_Y);
	std::cout << msg << std::flush;
}

void Screen::room1Challenge(char ch, point position, Player* p ){
	if (ch == '5'){
		showMessage("Congratulations! You have unlocked the correct door and may procceed to the next room");
		setChar(position, ' ');
		p->clearFromScreen();
		setSuccessfulMove(true);
	}
	else {
		showMessage("this lead to dead end! try another door");
		if (position.getX() == 0 || position.getX() == MAX_X-1) {
			setChar(position, '|');
		}
		else {
			setChar(position, '-');
		}
	}
}

void Screen::initaializeRoomsArray() {
	Rooms[MENU] = menu;
	Rooms[INSTRUCTIONS] = instructions;
	Rooms[ROOM1] = room1;
	Rooms[ROOM2] = room2;
}

bool Screen::tryPushObstacle(const point& obstaclePos, Keys direction, int force) {
	int obstacleWeight = 1;
	if (force < obstacleWeight) return false;
	point targetPos = obstaclePos;
	targetPos.setDirection(direction);
	targetPos.move();	
	char targetChar = getCharAt(targetPos);
	if (targetChar == ' ') {
		char objectChar = getCharAt(obstaclePos);
		setChar(obstaclePos, ' ');
		setChar(targetPos, objectChar);
		return true;
	}
	return false;
}
