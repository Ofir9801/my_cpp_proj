#include "Screen.h"
#include <iostream>
#include <windows.h>
#include "Utils.h"
#include "Player.h"
#include "Spring.h"
#include "Riddle.h"
#include "Bomb.h"
#include <random>
#include <string>
#include <algorithm>
#include <cctype> //  for tolower, isdigit

using std::cout;
using std::endl;
using std::string;

Screen::Screen() 
{
	initaializeRoomsArray();
	for (int i = 0; i < MAX_Y; i++) {
		map[i].resize(MAX_X, ' ');
	}
}
void Screen::loadMap(int roomNumber)
{
	for (int i = 0; i < MAX_Y; i++) {
		map[i] = Rooms[roomNumber][i];
	}
	currentRoom = roomNumber;
	if (roomNumber == roomIndex::ROOM3)
		isDarkRoom = true;
	else
		isDarkRoom = false;
	loadItems();
}
void Screen::drawMap() {
	cls(); //clear the console
	if (isDarkRoom) {
		for (int i = 0; i < 2; i++) {cout << map[i];}
	}
	else {
		for (int i = 0; i < MAX_Y; i++) {
			gotoxy(0, i);
			if (i > 2 && colorToggle) {
				for (int j = 0; j < MAX_X; j++) {
					char c = map[i][j];
					int color = getColorForChar(c);
					SetTextColor(color);
					cout << c;
				}
				SetTextColor(WHITE); //reset to default color
			}
			else {
				cout << map[i];
			}
		}
	}
}
void Screen::drawMap(int roomNumber) {
	cls(); //clear the console
	if (roomNumber == VICTORY &&colorToggle) {
		for (int i = 0; i < MAX_Y; i++) {
			gotoxy(0, i);
			if (i > 2) {
				for (int j = 0; j < MAX_X; j++) {
					char c = map[i][j];
					SetTextColor(BROWN);
					cout << c;
				}
				SetTextColor(WHITE); //reset to default color
			}
			else {
				cout << map[i];
			}
		}
	}
	else {drawMap();}
}

bool Screen::isWall(const Point& p) const{
	char c = getCharAt(p);
	return c == '-' || c == '|' || c == 'X';
}

void Screen::showPlayerInfo(const Player& p) {
	char playerChar = p.getChar();

	switch (playerChar) {
	case objSigns::PLAYER1:
		gotoxy(PLAYER1_SIGN_START_X, PLAYER_SIGN_Y);
		cout << playerChar << std::flush;
		gotoxy(PLAYER1_LIVES_START_X, PLAYER_SIGN_Y);
		cout << p.getLives() << std::flush;
		gotoxy(PLAYER1_INV_START_X, PLAYER_INV_Y);
		cout << p.getInventory() << std::flush;
		gotoxy(PLAYER1_SCORE_START_X, PLAYER_INV_Y);
		cout << p.getScore() << std::flush;
		break;
	case objSigns::PLAYER2:
		gotoxy(PLAYER2_SIGN_START_X, PLAYER_SIGN_Y);
		cout << playerChar << std::flush;
		gotoxy(PLAYER2_LIVES_START_X, PLAYER_SIGN_Y);
		cout << p.getLives() << std::flush;
		gotoxy(PLAYER2_INV_START_X, PLAYER_INV_Y);
		cout << p.getInventory() << std::flush;
		gotoxy(PLAYER2_SCORE_START_X, PLAYER_INV_Y);
		cout << p.getScore() << std::flush;
		break;
	}
}
void Screen::setChar(const Point& p, char c) {
	if (p.getX() < 0 || p.getX() >= MAX_X || p.getY() < 0 || p.getY() >= MAX_Y)
		return;
	map[p.getY()][p.getX()] = c;
	gotoxy(p.getX(), p.getY());
	if (colorToggle) {
		SetTextColor(getColorForChar(c));
		cout << c;
		SetTextColor(WHITE); //reset to default color
		return;
	}
	else
		cout << c;
}

void Screen::showKeyBinds() const
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
void Screen::showMessage(string msg){
	gotoxy(MESSAGES_POS::MES_X, MESSAGES_POS::MES_Y);
	std::cout << EMPTYLINE << std::flush;//clear the line before
	gotoxy(MESSAGES_POS::MES_X, MESSAGES_POS::MES_Y);
	std::cout << msg << std::flush;
}

void Screen::initaializeRoomsArray() {
	if (ReadRoomLayoutFromFile(MenuPathWay, roomIndex::MENU)){
		throw std::runtime_error("Something wrong with the file menu.txt");
	}
	if (ReadRoomLayoutFromFile(InstructionsPathWay, roomIndex::INSTRUCTIONS)) { 
		throw std::runtime_error("Something wrong with the file instructiopn.txt"); 
	}
	if (ReadRoomLayoutFromFile(Room1PathWay, roomIndex::ROOM1)){
		throw std::runtime_error("Something wrong with the file room1.txt"); 
	}
	if (ReadRoomLayoutFromFile(Room2PathWay, roomIndex::ROOM2)){ 
		throw std::runtime_error("Something wrong with the file room2.txt"); 
	}
	if (ReadRoomLayoutFromFile(Room3PathWay, roomIndex::ROOM3)) {
		throw std::runtime_error("Something wrong with the file room3.txt");
	}
	if (ReadRoomLayoutFromFile(EndingScreenPathWay, roomIndex::VICTORY)){
		throw std::runtime_error("Something wrong with the file endingscreen.txt"); 
	}	
	
	Rooms[MENU] = Menu;
	Rooms[INSTRUCTIONS] = Instructions;
	Rooms[ROOM1] = Room1;
	Rooms[ROOM2] = Room2;
	Rooms[ROOM3] = Room3;
	Rooms[VICTORY] = EndingScreen;
}

bool Screen::tryPushObstacle(const Point& obstaclePos, Keys direction, int force) {
	int chainLength = 0;
	Point scanner = obstaclePos;
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
			Point p(x, y);
			if (getCharAt(p) == objSigns::SPRING && !processed[y][x]) {
				bool isHorizontal = (x + 1 < MAX_X && getCharAt(Point(x + 1, y)) == objSigns::SPRING);
				if (!isHorizontal && (y + 1 >= MAX_Y || getCharAt(Point(x, y + 1)) != objSigns::SPRING)) {
					isHorizontal = true;
				}

				int length = 0;
				Keys dir = Keys::STAY;
				Point currentStart(x, y);

				if (isHorizontal) {//horizontal
					int currX = x;
					while (currX < MAX_X && getCharAt(Point(currX, y)) == objSigns::SPRING) {
						processed[y][currX] = true;
						length++;
						currX++;
					}
					if (isWall(Point(x - 1, y))) dir = Keys::RIGHT;
					else if (isWall(Point(x + length, y))) dir = Keys::LEFT;
					else if (isWall(Point(x, y - 1))) dir = Keys::DOWN;
					else if (isWall(Point(x, y + 1))) dir = Keys::UP;
				}
				else { //vertical
					int currY = y;
					while (currY < MAX_Y && getCharAt(Point(x, currY)) == objSigns::SPRING) {
						processed[currY][x] = true;
						length++;
						currY++;
					}
					// walls check
					if (isWall(Point(x, y - 1))) dir = Keys::DOWN;
					else if (isWall(Point(x, y + length))) dir = Keys::UP;
				}
				if (dir != Keys::STAY) {
					springs.push_back(Spring(currentStart, length, dir, *this));
				}
			}
		}
	}
}
Spring* Screen::getSpringAt(const Point& p) {
	for (auto& s : springs) {
		if (s.isPlayerOn(p)) {
			return &s;
		}
	}
	return nullptr;
}
void Screen::refreshSpringsDisplay(const Point& p1, const Point& p2) const {
	for (const auto& s : springs) {
		bool p1On = s.isPlayerOn(p1);
		bool p2On = s.isPlayerOn(p2);
		if (!p1On && !p2On) {
			s.draw(Point(0, 0), false);
		}
	}
}

void Screen::clearMessegeArea(int const counter)
{
	if (counter % 20 == 0) // clear message area every 20 cycles
		showMessage(EMPTYLINE);
}

void Screen::loadItems() {//enter the items from the board to the vector
	switches.clear();
	obstacles.clear();
	doors.clear();
	keys.clear();
	doorIDs.clear();
	for (int y = 3; y < BOARD_DIMENSION::MAX_Y; y++) {
		for (int x = 0; x < BOARD_DIMENSION::MAX_X; x++) {
			char c = getCharAt(Point(x, y));
			if (c == objSigns::SWITCH_OFF) {
				switches.push_back(Switch(x, y, this, false));
			}
			else if (c == objSigns::SWITCH_ON) {
				switches.push_back(Switch(x, y, this, true));
			}
			else if (c == objSigns::OBSTACLE) {
				obstacles.push_back(Obstacle(x, y, this, 1));
			}
			else if (isdigit((unsigned char)c)) {
				doorIDs.push_back(c - '0');
				doors.push_back(Door(x, y, c, this));
			}
			else if (c == objSigns::KEY) {
				keys.push_back(Key(x, y));
			}
			//else if (c == objSigns::RIDDLE) {
			//	//TODO: better riddle managment
			//	std::string q = "What is 2 + 2?";
			//	std::vector<std::string> options = { "3", "4", "5", "6" };
			//	int correctIndex = 1;
			//	riddles.push_back(Riddle(Point(x, y), q, options, correctIndex));
			//}
		}
		linkDoorsToKeysAndSwitches();
		loadSprings();
	}
}
	void Screen::linkDoorsToKeysAndSwitches() { //the assumption is that the number of switches and  is equal to the number of doors
	std::vector <int> doorIdCopy = doorIDs;  //make copy of doorIds vector 

	std::random_device rd;  //use gemini to get known of the shuffle algorithm and how to integrate it with the code
	std::mt19937 g(rd());	//the prompt is "give me idea to connect between doors id to switches and keys in randomize pattern in complexicity lower than o(n^2)

	std::shuffle(doorIdCopy.begin(), doorIdCopy.end(), g);
	for (int i = 0; i < keys.size(); i++) {
		if (i >= doorIdCopy.size())
			break;
		int currentDoorId = doorIdCopy[i];
		keys[i].setTargetDoorId(currentDoorId);
	}
	
	std::shuffle(doorIdCopy.begin(), doorIdCopy.end(), g);
	for (int i = 0; i < switches.size(); i++) {
		if (i >= doorIdCopy.size())
			break;
		int currentDoorId = doorIdCopy[i];
		switches[i].setTargetDoorId(currentDoorId);
		setconnection(currentDoorId);
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

void Screen::addKeyToInventory(Point position, char p){
	for (auto& k : keys) {
		if (k.getPosition() == position) {
			k.setInUse(true, p);
			break;
		}
	}
}

void Screen::RemoveKeyFromInventory(char p, Point newPos) {
	for (auto& k : keys) {
		if (k.getInUse() == true && k.getPlayerUse() == p) {
			k.setInUse(false, ' ');
			k.SetPosition(newPos);
			break;
		}
	}
}

int Screen::GetDoorIdByKey(char p) {
	for (auto& k : keys) {
		if (k.getPlayerUse() == p) {
			return k.getTargetDoorId();
			break;
		}
	}
	return -1;
}



bool Screen::handleRiddle(Player& p) {
	for (auto it = riddles.begin(); it != riddles.end(); ++it) {
		if (it->isSolved()) { continue; }
		else{
			bool solved = it->engage(*this, p);
			if (solved) {
				it->ChangeSolve(true);
				return true;
			}
			return false;
		}
	}
	return false;
}

void Screen::updateLighting(const Point& p1, const Point& p1Prev, const Player& player1, const Point& p2, const Point& p2Prev, const Player& player2)
{
	int r1 = player1.hasItem(objSigns::TORCH) ? LIGHT_RADIUS_TORCH : LIGHT_RADIUS_DEFAULT;
	int r2 = player2.hasItem(objSigns::TORCH) ? LIGHT_RADIUS_TORCH : LIGHT_RADIUS_DEFAULT;

	//erase old area
	ProcessLightning(p1Prev.getX(), p1Prev.getY(), LIGHT_RADIUS_TORCH, true,p1,p2,r1,r2);
	ProcessLightning(p2Prev.getX(), p2Prev.getY(), LIGHT_RADIUS_TORCH, true,p1,p2,r1,r2);

	ProcessLightning(p1.getX(), p1.getY(), r1, false, p1, p2, r1, r2);
	ProcessLightning(p2.getX(), p2.getY(), r2, false, p1, p2, r1, r2);

}

bool Screen::isLit(int x, int y, const Point& p, int radius) {
	int dx = x - p.getX(); //calculate the the distance between two points by x
	int dy = y - p.getY(); //calculate the the distance between two points by y
	return (dx * dx + dy * dy) <= (radius * radius); //true if the given point is in player's light radius
}

void Screen::ProcessLightning(int cx,int cy, int radius, bool erase, const Point& p1,const Point& p2, const int r1, const int r2) {
	for (int y = cy - radius; y <= cy + radius; y++) {
		for (int x = cx - radius; x <= cx + radius; x++) {
			if (x < 0 || x >= MAX_X || y < 3 || y >= MAX_Y)
				continue;

			bool inRange = isLit(x, y, Point(cx, cy), radius); //check if point in distance

			if (inRange) {
				if (erase) {
					if (!isLit(x, y, p1, r1) && !isLit(x, y, p2, r2)) {
						gotoxy(x, y);
						std::cout << ' ';
					}
				}
				else{
					gotoxy(x, y);
					char c = map[y][x];
					if (colorToggle) {
						SetTextColor(getColorForChar(c));
						std::cout << c;
						SetTextColor(WHITE);
					}
					else {
						std::cout << c;
					}
				}
			}
		}
	}
}

void Screen::updateBombs(Player& p1, Player& p2) {
	for (size_t i = 0; i < activeBombs.size(); ) {
		activeBombs[i].tick();
		if (activeBombs[i].shouldExplode()) {
			activeBombs[i].explode(*this, p1, p2);
			setChar(activeBombs[i].getPosition(), ' ');
			activeBombs.erase(activeBombs.begin() + i);
		}
		else {
			++i;
		}
	}
}

