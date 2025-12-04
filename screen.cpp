#include "Screen.h"
#include <iostream>
#include <windows.h>
#include "Utils.h"
#include "Rooms.h"
#include "Player.h"
#include "Spring.h"
#include <cctype> //  for tolower, isdigit

using std::cout;
using std::endl;

Screen::Screen() 
{
	loadItems();
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
	loadSprings();
	loadItems();
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
	return c == '-' || c == '|' || c == 'X';
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
	std::cout << EMPTYLINE << std::flush;//clear the line before
	gotoxy(MESSAGES_POS::MES_X, MESSAGES_POS::MES_Y);
	std::cout << msg << std::flush;
}

void Screen::initaializeRoomsArray() {
	Rooms[MENU] = menu;
	Rooms[INSTRUCTIONS] = instructions;
	Rooms[ROOM1] = room1;
	Rooms[ROOM2] = room2;
	Rooms[VICTORY] = endingScreen;
}

bool Screen::tryPushObstacle(const point& obstaclePos, Keys direction, int force) {
	int chainLength = 0;
	point scanner = obstaclePos;
	while (getCharAt(scanner) == '*') {
		chainLength++;
		scanner.setDirection(direction);
		scanner.move();
		if (scanner.getX() < 0 || scanner.getX() >= MAX_X || scanner.getY() < 0 || scanner.getY() >= MAX_Y) break;
	}
	if (force < chainLength) return false; // Not enough force to push the entire chain
	char charBehind = getCharAt(scanner);
	if (charBehind != ' ') return false;
	setChar(obstaclePos, ' '); // Clear the first obstacle position
	setChar(scanner, '*'); // Set the last position to an obstacle
	return true;
}

void Screen::loadSprings() {
	springs.clear();
	bool processed[BOARD_DIMENSION::MAX_Y][BOARD_DIMENSION::MAX_X] = { false };
	for (int y = 0; y < BOARD_DIMENSION::MAX_Y; y++) {
		for (int x = 0; x < BOARD_DIMENSION::MAX_X; x++) {
			point p(x, y);
			if (getCharAt(p) == objSigns::SPRING && !processed[y][x]) {
				bool isHorizontal = (x + 1 < MAX_X && getCharAt(point(x + 1, y)) == objSigns::SPRING);
				if (!isHorizontal && (y + 1 >= MAX_Y || getCharAt(point(x, y + 1)) != objSigns::SPRING)) {
					isHorizontal = true;
				}

				int length = 0;
				Keys dir = Keys::STAY;
				point currentStart(x, y);

				if (isHorizontal) {//horizontal
					int currX = x;
					while (currX < MAX_X && getCharAt(point(currX, y)) == objSigns::SPRING) {
						processed[y][currX] = true;
						length++;
						currX++;
					}
					if (isWall(point(x - 1, y))) dir = Keys::RIGHT;
					else if (isWall(point(x + length, y))) dir = Keys::LEFT;
					else if (isWall(point(x, y - 1))) dir = Keys::DOWN;
					else if (isWall(point(x, y + 1))) dir = Keys::UP;
				}
				else { //vertical
					int currY = y;
					while (currY < MAX_Y && getCharAt(point(x, currY)) == objSigns::SPRING) {
						processed[currY][x] = true;
						length++;
						currY++;
					}
					// walls check
					if (isWall(point(x, y - 1))) dir = Keys::DOWN;
					else if (isWall(point(x, y + length))) dir = Keys::UP;
				}
				if (dir != Keys::STAY) {
					springs.push_back(Spring(currentStart, length, dir, *this));
				}
			}
		}
	}
}
Spring* Screen::getSpringAt(const point& p) {
	for (auto& s : springs) {
		if (s.isPlayerOn(p)) {
			return &s;
		}
	}
	return nullptr;
}
void Screen::refreshSpringsDisplay(const point& p1, const point& p2) const {
	for (const auto& s : springs) {
		bool p1On = s.isPlayerOn(p1);
		bool p2On = s.isPlayerOn(p2);
		if (!p1On && !p2On) {
			s.draw(point(0, 0), false);
		}
	}
}

void Screen::clearMessegeArea(int const counter)
{
	if (counter % 20 == 0) // clear message area every 10 cycles
		showMessage(EMPTYLINE);
}

void Screen::loadItems() {//enter the items from the board to the vector
	switches.clear();
	obstacles.clear();
	doors.clear();
	for (int y = 0; y < BOARD_DIMENSION::MAX_Y; y++) {
		for (int x = 0; x < BOARD_DIMENSION::MAX_X; x++) {
			char c = getCharAt(point(x, y));
			if (c == '\\') {
				switches.push_back(Switch(x, y, this, false));
			}
			else if (c == '/') {
				switches.push_back(Switch(x, y, this, true));
			}
			else if (c == '*') {
				obstacles.push_back(Obstacle(x, y, this, 1));
			}
			else if (isdigit((unsigned char)c)) {
				doors.push_back(Door(x, y, c, this));
			}
		}
	}
	autoLinkSwitchesAndDoors();
}
void Screen::autoLinkSwitchesAndDoors() {
	int levelNum = getCurrentRoom() - 1;
	int currentDoorId = 1;
	for (auto& s : switches) {
		s.setTargetDoorId(currentDoorId);
		setconnection(currentDoorId);
		currentDoorId++;
	}
}
bool Screen::isDoorOpen(int door_id)
{
	for (const auto& d : doors) {
		if (d.getId() == door_id) {
			return d.getIsOpen();
		}
	}
}
void Screen::openDoor(int door_id)
{
	for (auto& d : doors) {
		if (d.getId() == door_id) {
			d.open();
		}
	}
}

void Screen::setconnection(int doorId) {
	for (auto& d : doors) {
		if (d.getId() == doorId) {
			d.setConnection(true);
		}
	}
}

bool Screen::ConnectionStatus(int doorId) {
	for (const auto& d : doors) {
		if (d.getId() == doorId) {
			return d.getConnection();
		}
	}
}

bool Screen::SwitchState(int doorId) {
	for (auto& s : switches) {
		if (s.getTargetDoorId() == doorId)
			return s.getIsOn();
	}
}