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
#include <fstream>
#include <cctype> //  for tolower, isdigit

using std::cout;
using std::endl;
using std::string;

Screen::Screen() 
{
	initaializeRoomsArray();
	for (int i = 0; i < MAX_Y; i++) {
		board[i].resize(MAX_X, ' ');
	}
}
void Screen::loadMap(int roomNumber)
{
	for (int i = 0; i < MAX_Y; i++) {
		board[i] = Rooms[roomNumber][i];
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
		for (int i = 0; i < 2; i++) {cout << board[i];}
	}
	else {
		for (int i = 0; i < MAX_Y; i++) {
			gotoxy(0, i);
			if (i > 2 && colorToggle) {
				for (int j = 0; j < MAX_X; j++) {
					char c = board[i][j];
					int color = getColorForChar(c);
					SetTextColor(color);
					cout << c;
				}
				SetTextColor(WHITE); //reset to default color
			}
			else {
				cout << board[i];
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
					char c = board[i][j];
					SetTextColor(BROWN);
					cout << c;
				}
				SetTextColor(WHITE); //reset to default color
			}
			else {
				cout << board[i];
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
		gotoxy(PLAYER1_SIGN_START_X, PLAYER_SIGN_Y); //print player char
		cout << playerChar << std::flush;
		gotoxy(PLAYER1_INV_START_X, PLAYER_INV_Y); //print inventory
		cout << p.getInventory() << std::flush;
		break;
	case objSigns::PLAYER2:
		gotoxy(PLAYER2_SIGN_START_X, PLAYER_SIGN_Y); //print player char
		cout << playerChar << std::flush;
		gotoxy(PLAYER2_INV_START_X, PLAYER_INV_Y);//print inventory
		cout << p.getInventory() << std::flush;
		break;
	}
	gotoxy(SCORE_START_X, PLAYER_SIGN_Y); //print score
	cout << p.getLives() << std::flush;
	gotoxy(LIVES_START_X, PLAYER_SIGN_Y); //print lives
	cout << p.getScore() << std::flush;
}
void Screen::setChar(const Point& p, char c) {
	if (p.getX() < 0 || p.getX() >= MAX_X || p.getY() < 0 || p.getY() >= MAX_Y)
		return;
	board[p.getY()][p.getX()] = c;
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

bool Screen::tryPushObstacle(const Point& obstaclePos, Keyboard_bind direction, int force) {
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
				Keyboard_bind dir = Keyboard_bind::STAY;
				Point currentStart(x, y);

				if (isHorizontal) {//horizontal
					int currX = x;
					while (currX < MAX_X && getCharAt(Point(currX, y)) == objSigns::SPRING) {
						processed[y][currX] = true;
						length++;
						currX++;
					}
					if (isWall(Point(x - 1, y))) dir = Keyboard_bind::RIGHT;
					else if (isWall(Point(x + length, y))) dir = Keyboard_bind::LEFT;
					else if (isWall(Point(x, y - 1))) dir = Keyboard_bind::DOWN;
					else if (isWall(Point(x, y + 1))) dir = Keyboard_bind::UP;
				}
				else { //vertical
					int currY = y;
					while (currY < MAX_Y && getCharAt(Point(x, currY)) == objSigns::SPRING) {
						processed[currY][x] = true;
						length++;
						currY++;
					}
					// walls check
					if (isWall(Point(x, y - 1))) dir = Keyboard_bind::DOWN;
					else if (isWall(Point(x, y + length))) dir = Keyboard_bind::UP;
				}
				if (dir != Keyboard_bind::STAY) {
					springs.push_back(Spring(currentStart, length, dir));
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
			s.draw(Point(0, 0),*this, false);
		}
	}
}

void Screen::clearMessegeArea(int const counter)
{
	if (counter % 40 == 0) // clear message area every 40 cycles
		showMessage(EMPTYLINE);
}

void Screen::loadItems() {//enter the items from the board to the appropiete data structures
	switches.clear();
	obstacles.clear();
	doors.clear();
	doorIDs.clear();
	keys.clear();
	riddles.clear();
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
				int door_id = c - '0';
				doors[door_id] = Door(x, y, c);
				doorIDs.push_back(door_id);
			}
			else if (c == objSigns::KEY) {
				keys[Point(x, y)] = Key(x, y);
			}
			else if (c == objSigns::RIDDLE) {
				riddles[Point(x, y)] = Riddle(Point(x, y,objSigns::RIDDLE));
			}
		}
	}
	linkDoorsToKeysAndSwitches();
	loadRiddles();
	loadSprings();
}

void Screen::linkDoorsToKeysAndSwitches() { //the assumption is that the number of switches and  is equal to the number of doors
	std::vector <int> doorIdCopy = doorIDs;  //make copy of doorIds vector 

	std::random_device rd;  //use gemini to get known of the shuffle algorithm and how to integrate it with the code
	std::mt19937 g(rd());	//the prompt is "give me idea to connect between doors id to switches and keys in randomize pattern in complexicity lower than o(n^2)

	int currentIndex = 0;
	std::shuffle(doorIdCopy.begin(), doorIdCopy.end(), g);
	auto it = keys.begin();
	while (it != keys.end() && currentIndex < doorIdCopy.size()) {
			int currentDoorId = doorIdCopy[currentIndex];
			it->second.setTargetDoorId(currentDoorId);
			++it;
			currentIndex++;
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

bool Screen::isDoorOpen(int door_id){
	auto it = doors.find(door_id);
	if (it != doors.end()) {
		return it->second.getIsOpen();
	}
	return false;
}

void Screen::openDoor(int door_id){
	auto it = doors.find(door_id);
	if (it != doors.end()) {
		it->second.open();
		showMessage("Door opened!");
	}
}

void Screen::setconnection(int door_id) {
	auto it = doors.find(door_id);
	if (it != doors.end()) {
		it->second.setConnection(true);
	}
}

bool Screen::ConnectionStatus(int door_id) {
	auto it = doors.find(door_id);
	if (it != doors.end()) {
		return it->second.getConnection();
	}
	return false;
}

bool Screen::SwitchState(int doorId) {
	for (auto& s : switches) {
		if (s.getTargetDoorId() == doorId)
			return s.getIsOn();
	}
	return false;
}

void Screen::addKeyToInventory(Point position, char p){
	auto it = keys.find(position);
	if (it != keys.end()) {
		it->second.setInUse(p);
	}
}

void Screen::RemoveKeyFromInventory(char p, Point newPos) {
	auto it = keys.begin();
	while (it != keys.end())
	{
		if (it->second.getPlayerUse() == p) {
			Key tempKey = it->second;
			keys.erase(it);
			tempKey.setInUse(' ');
			tempKey.SetPosition(newPos);
			keys[newPos] = tempKey;
			return;
		}
		else {
			++it;
		}
	}
}

int Screen::GetDoorIdByKey (char p) const{
	for (auto& k : keys) {
		if (k.second.getPlayerUse() == p) {
			return k.second.getTargetDoorId();
		}
	}
	return -1;
}

bool Screen::handleRiddle(Point riddlePos, Player& p) {
	auto it = riddles.find(riddlePos);
	if (it != riddles.end() && !it->second.isSolved()) {
		bool solved = riddles[riddlePos].engage(p);
		drawMap(); //redraw the map after riddle engagement
		if (solved) {
			string msg = "Correct! +" + std::to_string(SUCCESS_SCORE) + "points!";
			showMessage(msg);
			//setChar(riddlePos, ' '); //remove riddle from the board
			return true;
		}
		else {
			string msg = "WRONG! You lost" + std::to_string(WRONG_ANSWER) + " points!";
			showMessage(msg);
			return false;
		}
	}
	else if (it != riddles.end()) {
		string msg = "You Already solved this riddle, the correct answer was " + std::to_string(it->second.getCorrectIndex()+1)+"." + it->second.getCorrectAnswer();
		showMessage(msg);
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

bool Screen::Distance(int x, int y, const Point& p, int radius) {
	int dx = x - p.getX(); //calculate the the distance between two points by x
	int dy = y - p.getY(); //calculate the the distance between two points by y
	return (dx * dx + dy * dy) <= (radius * radius); //true if the given point is in player's light radius
}

void Screen::ProcessLightning(int cx,int cy, int radius, bool erase, const Point& p1,const Point& p2, const int r1, const int r2) {
	for (int y = cy - radius; y <= cy + radius; y++) {
		for (int x = cx - radius; x <= cx + radius; x++) {
			if (x < 0 || x >= MAX_X || y < 3 || y >= MAX_Y)
				continue;

			bool inRange = Distance(x, y, Point(cx, cy), radius); //check if point in distance

			if (inRange) {
				if (erase) {
					if (!Distance(x, y, p1, r1) && !Distance(x, y, p2, r2)) {
						gotoxy(x, y);
						std::cout << ' ';
					}
				}
				else{
					if ((x == p1.getX() && y == p1.getY()) || (x == p2.getX() && y == p2.getY())){
						continue;
					}
					gotoxy(x, y);
					char c = board[y][x];
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

bool Screen::isValid(const Point& p) const{
	int x = p.getX();
	int y = p.getY();
	char c = board[y][x];
	bool door = isdigit(c);
	bool insideBoard = x > 0 && x < MAX_X - 1 && y > 3 && y < MAX_Y - 1;
	return door ||insideBoard;
}

void Screen::deleteKey(Point position){
	auto it = keys.find(position);
	if (it != keys.end()) {
		keys.erase(it);
	}
}

void Screen::deleteSpring(Point position){
	for (size_t i = 0; i < springs.size(); ++i) {
		if (springs[i].isPointOn(position)) {
			int hitIndex = springs[i].getSegmentIndex(position);
			springs[i].setLength(hitIndex);
			if (springs[i].getLength() <= 0) {
				springs.erase(springs.begin() + i);
			}
			return;
		}
	}
}

void Screen::deleteSwitch(Point position){
	bool flag = false;
	std::vector<Switch>::iterator it = switches.begin();
	while (it != switches.end() && !flag) {
		if (it->getPosition() == position)
		{
			it = switches.erase(it);
			flag = true;
		}
		else { ++it; }
	}
}

void Screen::deleteDoor(Point position){
	int door_id = position.getChar() - '0';
	auto it = doors.find(door_id);
	if (it != doors.end()) {
		if (it->second.getId() == getCurrentRoom() - 1){ //explode winning door
			gameState = false;
		}
		doors.erase(it);
	}
}

bool Screen::isBombAt(const Point& p) const{
	for (const auto& bomb : activeBombs) {
		if (bomb.getPosition() == p) {
			return true;
		}
	}
	return false;
}

void Screen::updateBombs(Player& p1, Player& p2) {
	for (size_t i = 0; i < activeBombs.size(); ) {
		activeBombs[i].tick();
		int time = activeBombs[i].getTimer();
		setChar(activeBombs[i].getPosition(),'0' + time / 10);
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
void Screen::decreaseLife() {
	if (sharedLives > 0) {
		sharedLives--;
		showMessage("Life lost, be carefull! Lives left: " + std::to_string(sharedLives));
	}
	else
	{
		gameState = false;
		showMessage("No lives left! Game Over!");
		cls();
	}
}

void Screen::loadRiddles(){
	auto it = riddles.begin();
	int counter = 0;
	
	while (it != riddles.end())
	{
		bool error = false;
		Riddle temp;
		switch (currentRoom) {
			case roomIndex::ROOM1:
				temp = ReadRiddleFromFile(RiddlesRoom1PathWay,it->first, counter, error);
			break;
			case roomIndex::ROOM2:
				temp = ReadRiddleFromFile(RiddlesRoom2PathWay, it->first, counter, error);
				break;
			case roomIndex::ROOM3:
				temp = ReadRiddleFromFile(RiddlesRoom3PathWay, it->first, counter, error);
				break;
		}
		
		if (error) {
			throw std::runtime_error("Something wrong with the file riddle.txt");
		}
		else {
			it->second = std::move(temp);
			++it;
			counter++;
		}
	}
}

Riddle Screen::ReadRiddleFromFile(const string& filePath,const Point pos, int riddleIndex, bool& error){
	std::ifstream inFile(filePath);
	std::string question;
	std::vector<std::string> options;
	int correctIndex = 0;

	if (!inFile.is_open()) {
		error = true;
		return Riddle();
	}
	string templine;
	for (int i = 0; i < riddleIndex; i++) { //skip to the right riddle index
		for (int j = 0; j < 6; j++) { //every riddle is represnt by 6 lines in riddle text file
			if (!std::getline(inFile, templine)) {
				error = true;
				return Riddle();
			}
		}
	}

	for (int i = 0; i < 6; i++) {
		if (!std::getline(inFile, templine)) {
			error = true;
			return Riddle();
		}
		if (!templine.empty() && templine.back() == '\r') {
			templine.pop_back(); // Remove carriage return character if present
		}
		if (i == 0) { question = templine; } //first line is the question
		else if (i == 1) { 
			correctIndex = std::stoi(templine) - 1 ;//second line is the right answer index
		}				 
		else { options.push_back(templine); } // other 3 lines is wrong answers
	}
	inFile.close();

	return Riddle(pos, question, options, correctIndex);
}



