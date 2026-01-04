#include "Bomb.h"
#include "Player.h"
#include "Riddle.h"
#include "Screen.h"
#include "Spring.h"
#include "Utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <windows.h>
#include <conio.h>

using std::cout;
using std::endl;
using std::string;

Screen::Screen() {
	initializeRoomsArray();
	for (int i = 0; i < MAX_Y; i++) {
		board[i].resize(MAX_X, ' ');
	}
	RiddlePathWays.clear();
	getAllFilePaths(RiddlePathWays, RiddlesExtension, RiddlesFolder);
	currentRoom = static_cast<int>(roomIndex::INSTRUCTIONS);//when start the game the first screen is menu
}

void Screen::loadMap(int roomNumber, Point& doorPos){
	int lastRoom = currentRoom;
	currentRoom = roomNumber;
	const int legendY = roomLegendRows[currentRoom];

	if (savedRooms.find(roomNumber)!=savedRooms.end()) { //load saved state
		for (int i = 0; i < MAX_Y; i++) {
			board[i] = savedRooms[currentRoom].layout[i];
		}
		springs = savedRooms[currentRoom].springs;
		switches = savedRooms[currentRoom].switches;
		obstacles = savedRooms[currentRoom].obstacles;
		doors = savedRooms[currentRoom].doors;
		keys = savedRooms[currentRoom].keys;
		activeBombs = savedRooms[currentRoom].activeBombs;
		riddles = savedRooms[currentRoom].riddles;
		
		if (lastRoom != roomIndex::MENU) {
			char targetDoorChar = static_cast<char>('0' + lastRoom); // e.g., if coming from Room 1, look for '1'
			bool found = false;

			for (int y = 0; y < MAX_Y && !found; y++) {
				if (inLegendBounds(legendY, y)) { continue; }
				for (int x = 0; x < MAX_X && !found; x++) {
					if (board[y][x] == targetDoorChar) {
						doorPos.setX(x);
						doorPos.setY(y);
						found = true;
					}
				}
			}
		}
	}
	else { //first time loading the room
		for (int i = 0; i < MAX_Y; i++) {
			board[i] = Rooms[roomNumber][i];
		}
		if (lastRoom != roomIndex::MENU) {
			loadItems(lastRoom, doorPos);
		}
		else {
			loadItems(-1, doorPos); //no door to open
		}
		
	}
	if (roomNumber == roomIndex::ROOM3)
		isDarkRoom = true;
	else
		isDarkRoom = false;
	if (roomNumber == roomIndex::VICTORY && colorToggle) {
		drawVictoryRoom();
	}
	else {
		drawMap();
	}
	
}

void Screen::drawMap() {
	cls(); //clear the console
	const int legendY = roomLegendRows[currentRoom];
	if (isDarkRoom) {
		if (legendY != -1) {
			for (int i = 0; i < LEGEND_SIZE; i++) {
				gotoxy(0, legendY + i);
				std::cout << board[legendY + i];
			}
		}
	}
	else {
		for (int i = 0; i < MAX_Y; i++) {
			gotoxy(0, i);
			if (!inLegendBounds(legendY,i) && colorToggle) {
				for (int j = 0; j < MAX_X; j++) {
					char c = board[i][j];
					Color color = getColorForChar(c);
					SetTextColor(color);
					cout << c;
				}
				SetTextColor(Color::WHITE); //reset to default color
			}
			else {
				cout << board[i];
			}
		}
	}
}

void Screen::drawVictoryRoom() {
	cls();
	for (int i = 0; i < MAX_Y; i++) {
		gotoxy(0, i);
		if (i > 2) {
			for (int j = 0; j < MAX_X; j++) {
				char c = board[i][j];
				SetTextColor(Color::BROWN);
				cout << c;
			}
			SetTextColor(Color::WHITE); //reset to default color
		}
		else {
			cout << board[i];
		}
		string msg = "FINAL TEAM SCORE: " + std::to_string(sharedScore);
		gotoxy(X_SCORE_PRINT, Y_SCORE_PRINT);
		showMessage(msg);
	}
}

bool Screen::isWall(const Point& p) const {
	int legendY = roomLegendRows[currentRoom];

	if (inLegendBounds(legendY, p.getY())) {
		return true;
	}

	char c = getCharAt(p);
	return c == '-' || c == '|' || c == 'X';
}

string Screen::CreateInventoryDisplay(const Player & p) {
	string inventory = p.getInventory();
	string result = "|";
	for (size_t i = 0; i < inventory.size(); i++) {
			result += inventory[i];
			result += "|";
	}
	if (!p.isExtraInventory()) {
		result.resize(3);
	}
	return result;
}

void Screen::showPlayerInfo(const Player& p) {
	objSigns playerChar = (objSigns)p.getChar();
	string inventory = CreateInventoryDisplay(p);
	int legendY = roomLegendRows[currentRoom];
	if (legendY == -1) { return; }

	switch (playerChar) {
	case objSigns::PLAYER1:
		gotoxy(INFO_SLOTS::PLAYER1_SIGN_START_X, legendY); //print player char
		cout << playerChar << std::flush;
		gotoxy(INFO_SLOTS::PLAYER1_INV_START_X, legendY + 1); //print inventory
		cout << inventory << std::flush;
		break;
	case objSigns::PLAYER2:
		gotoxy(INFO_SLOTS::PLAYER2_SIGN_START_X, legendY); //print player char
		cout << playerChar << std::flush;
		gotoxy(INFO_SLOTS::PLAYER2_INV_START_X, legendY +1);//print inventory
		cout << inventory << std::flush;
		break;
	}
	gotoxy(INFO_SLOTS::SCORE_START_X, legendY); //print score
	cout << p.getLives() << std::flush;
	gotoxy(INFO_SLOTS::LIVES_START_X, legendY); //print lives
	cout << p.getScore() << std::flush;
}

void Screen::setChar(const Point& p, char c) {
	if (!p.InBounds())
		return;
	int legendY = roomLegendRows[currentRoom];
	if (inLegendBounds(legendY, p.getY())) {
		return;
	}

	board[p.getY()][p.getX()] = c;
	gotoxy(p.getX(), p.getY());
	if (colorToggle) {
		SetTextColor(getColorForChar(c));
		cout << c;
		SetTextColor(Color::WHITE); //reset to default color
		return;
	}
	else
		cout << c;
}
void Screen::setChar(const Point& p, objSigns sign) {
	char c = static_cast<char>(sign);
	setChar(p, c);
}

void Screen::showInstructionBinds() const
{
	int const OBJ_Y = 7;
	int const OBJ_X = 1;
	gotoxy(OBJ_X, OBJ_Y);
	cout << objSigns::KEY << endl;
	gotoxy(OBJ_X, OBJ_Y+1);
	cout << objSigns::SWITCH_OFF << endl;
	gotoxy(OBJ_X, OBJ_Y+3);
	cout << objSigns::BOMB << endl;
	gotoxy(OBJ_X, OBJ_Y+6);
	cout << objSigns::SPRING << endl;

	int const KEYS_Y = 19;
	int const KEYS_X1 = 11;
	int const KEYS_X2 = 40;

	for (int i = 0; i < NUM_KEYS; i++) {
		gotoxy(KEYS_X1, KEYS_Y + i);
		cout << (unsigned char)toupper(keys1[i]); //print uppercase
		gotoxy(KEYS_X2, KEYS_Y + i);
		cout << (unsigned char)toupper(keys2[i]); //print uppercase
	}
}
void Screen::showMessage(string msg){
	gotoxy(MESSAGES_POS::MES_X, roomLegendRows[currentRoom] + MESSAGES_POS::MES_Y);
	string extraSpace(MAX_X - msg.length(), ' ');
	std::cout << msg <<extraSpace <<std::flush;
	MessageTimer = TIMER_MESSAGE;
}

void Screen::initializeRoomsArray() {
	loadFilesByType(true);

	Rooms[static_cast<int>(roomIndex::MENU)] = Menu;
	Rooms[static_cast<int>(roomIndex::INSTRUCTIONS)] = Instructions;
	Rooms[static_cast<int>(roomIndex::ROOM1)] = Room1;
	Rooms[static_cast<int>(roomIndex::ROOM2)] = Room2;
	Rooms[static_cast<int>(roomIndex::ROOM3)] = Room3;
	Rooms[static_cast<int>(roomIndex::VAULT)] = Vault;
	Rooms[static_cast<int>(roomIndex::VICTORY)] = EndingScreen;
}

void Screen::loadFilesByType(bool type) {
	string errorMsg;
	while (true) {
		//string errorMsg = ReadRoomLayoutFromFile(fileName, room);
		if (type) //true = read room, false = read riddle
			errorMsg = ReadRoomFromFile();
		else
			errorMsg = loadRiddles();
		if (errorMsg.empty())
			break;
		cls();
		std::cout << "########################################################" << std::endl;
		std::cout << "ERROR LOADING Game" << std::endl;
		std::cout << "########################################################" << std::endl;
		std::cout << errorMsg << std::endl << std::endl;
		std::cout << "1. Fix the file externally." << std::endl;
		std::cout << "2. Press 'r' to RETRY." << std::endl;
		std::cout << "3. Press 'ESC' to EXIT Game." << std::endl;
		char c = _getch();
		if (c == ESC) {
			throw std::runtime_error("Game stopped by user due to file error");
		}
	}
}

Obstacle* Screen::getObstacleAt(const Point& p) {
	for (auto& obs : obstacles) {
		if (obs.getPosition() == p) {
			return &obs;
		}
	}
	return nullptr;
}

void Screen::loadSprings() {
	springs.clear();
	bool processed[MAX_Y][MAX_X] = { false };
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			Point p(x, y);
			if (getCharAt(p) == objSigns::SPRING && !processed[y][x]) {
				Spring* newSpring = Spring::CreateFromMap(*this, x, y, processed);
				if (newSpring != nullptr) {
					springs.push_back(*newSpring);
					delete newSpring; // Clean up the dynamically allocated memory
				}
			}
		}
	}
}
Spring* Screen::getSpringAt(const Point& p){
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

void Screen::clearMessegeArea(bool forceClean){
	if (forceClean) {
		gotoxy(MESSAGES_POS::MES_X, roomLegendRows[currentRoom] + MESSAGES_POS::MES_Y);
		std::cout << EMPTYLINE << std::flush;
	}
	else {
		if (MessageTimer > 0) {
			MessageTimer--;
			if (MessageTimer == 0) {
				gotoxy(MESSAGES_POS::MES_X, roomLegendRows[currentRoom] + MESSAGES_POS::MES_Y);
				std::cout << EMPTYLINE << std::flush;
			}
		}
	}
}

void Screen::loadItems(int doorIdOpen, Point&doorPos) {//enter the items from the board to the appropiete data structures
	switches.clear();
	obstacles.clear();
	doors.clear();
	doorIDs.clear();
	keys.clear();
	riddles.clear();
	int legendY = roomLegendRows[currentRoom];
	for (int y = 0; y < MAX_Y; y++) { 
		if (inLegendBounds(legendY,y)) { //skip legend lines from scanning
			continue;
		}
		for (int x = 0; x < MAX_X; x++) {
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
			else if (isdigit(static_cast<unsigned char>(c))) {
				int door_id = c - '0';
				if (door_id == 9) { door_id = 0; }
				doors[door_id] = Door(x, y, c);
				if (door_id == doorIdOpen) {
					doors[door_id].open();
					doorPos.setX(x);
					doorPos.setY(y);
				}
				else {
					doorIDs.push_back(door_id);
				}
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
	loadFilesByType(false); //loading riddles from files
	loadSprings();
}

void Screen::linkDoorsToKeysAndSwitches() { //the assumption is that the number of switches and  is equal to the number of doors
	std::vector <int> doorIdCopy = doorIDs;  //make copy of doorIds vector 

	std::random_device rd;  //use gemini to get known of the shuffle algorithm and how to integrate it with the code
	std::mt19937 g(rd());	//the prompt is "give me idea to connect between doors id to switches and keys in randomize pattern in complexicity lower than o(n^2)
	//logic: Use std::shuffle to randomize connections between doors, keys, and switches.
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
		setConnection(currentDoorId);
	}
}

bool Screen::isDoorOpen(int door_id) const{
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

void Screen::setConnection(int door_id) {
	auto it = doors.find(door_id);
	if (it != doors.end()) {
		it->second.setConnection(true);
	}
}

bool Screen::getConnectionStatus(int door_id) const{
	auto it = doors.find(door_id);
	if (it != doors.end()) {
		return it->second.getConnection();
	}
	return false;
}

bool Screen::SwitchState(int doorId) const{
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

void Screen::DisposeKeyToScreen(char p, Point newPos) {
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
void Screen::RemoveKey(int doorId) {
	auto it = keys.begin();
	while (it != keys.end()){
		if (it->second.getTargetDoorId() == doorId) {
			keys.erase(it);
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
			string msg = "Correct! + " + std::to_string(SUCCESS_SCORE) + " points!";
			showMessage(msg);
			return true;
		}
		else {
			string msg = "WRONG! You lost " + std::to_string(WRONG_ANSWER) + " points!";
			showMessage(msg);
			return false;
		}
	}
	else if (it != riddles.end()) {//stating the correct answer for vault purposes
		string msg = "You Already solved this riddle, the correct answer was " + std::to_string(it->second.getCorrectIndex()+1)+"." + it->second.getCorrectAnswer();
		showMessage(msg);
	}
	return false;
}

bool Screen::handleVaultRiddle(Point riddlePos) {
	auto it = riddles.find(riddlePos);
	if (it != riddles.end() && !it->second.isSolved()) {
		bool solved = riddles[riddlePos].engageVaultRiddle();
		drawMap(); //redraw the map after riddle engagement
		if (solved) {
			string msg = "Congratulations! You solved the last challenge. Proceed to the final door";
			setChar(riddlePos, objSigns::EMPTY);
			showMessage(msg);
			return true;
		}
		else {
			string msg = "WRONG! Check your answers and come back again";
			showMessage(msg);
			return false;
		}
	}
	else if (it != riddles.end()) {
		string msg = "You Already solved this riddle, proceed to the final door";
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

bool Screen::BoxDistance(int x, int y, const Point& p, int radius) const {
	int dx =std::abs(x - p.getX()); //calculate the distance between two points by x
	int dy = std::abs(y - p.getY()); //calculate the distance between two points by y
	return dx <= radius && dy <= radius;
	//return (dx * dx + dy * dy) <= (radius * radius); //true if the given point is in player's light radius
}
bool Screen::RoundDistance(int x, int y, const Point& p, int radius) const {
	int dx = x - p.getX(); //calculate the distance between two points by x
	int dy = y - p.getY(); //calculate the distance between two points by y
	return (dx * dx + dy * dy) <= (radius * radius); //true if the given point is in player's light radius
}


void Screen::ProcessLightning(int cx,int cy, int radius, bool erase, const Point& p1,const Point& p2, const int r1, const int r2) {
	int legendY = roomLegendRows[currentRoom];
	for (int y = cy - radius; y <= cy + radius; y++) {
		for (int x = cx - radius; x <= cx + radius; x++) {
			if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y)
				continue;
			if (inLegendBounds(legendY, y)) {
				continue;
			}
			bool inRange = RoundDistance(x, y, Point(cx, cy), radius); //check if point in distance

			if (inRange) {
				if (erase) {
					if (!RoundDistance(x, y, p1, r1) && !RoundDistance(x, y, p2, r2)) {
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
						SetTextColor(Color::WHITE);
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
	if (!p.InBounds(MAX_X - 1, MAX_Y - 1, 0, 0))
		return false;
	if (!isDestructible(p)) {
		return false;
	}
	return true;
}

bool Screen::isDestructible(const Point& p) const
{
	const int px = p.getX();
	const int py = p.getY();

	//check if point is in legend
	int legendY = roomLegendRows[currentRoom];
	if (inLegendBounds(legendY, py)) {
		return false;
	}

	//check horizontal exterior walls (left and right)
	if (px == 0 || px == MAX_X - 1) {
		return false;
	}
	//check verticl exterior walls (top and buttom)
	int topWallY = 0;
	int bottomWallY = MAX_Y - 1;
	if (legendY != -1) {
		if (legendY == 0) {
			topWallY = LEGEND_SIZE;
		}
		else if (legendY + LEGEND_SIZE >= MAX_Y) {
			bottomWallY = legendY - 1;
		}
	}
	if (py == topWallY || py == bottomWallY) {
		return false;
	}
	return true;

}

bool Screen::inLegendBounds(const int legendY, const int y) const
{
	if (legendY != -1) {
		return y >= legendY && y < legendY + LEGEND_SIZE;
	}
	return false;
}

void Screen::deleteKey(Point position){
	auto it = keys.find(position);
	if (it != keys.end()) {
		CheckExplodeNecessaryObject(it->second.getTargetDoorId());
		keys.erase(it);
	}
}

void Screen::deleteSpring(Point position){
	for (size_t i = 0; i < springs.size(); ++i) {
		if (springs[i].isPointOn(position)) {
			bool destroyed = springs[i].handleExplosion(position,*this);
			if (destroyed) {
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
			CheckExplodeNecessaryObject(it->getTargetDoorId());
			it = switches.erase(it);
			flag = true;
		}
		else { ++it; }
	}
}

void Screen::deleteDoor(Point position){
	int door_id = position.getChar() - '0';
	auto it = doors.find(door_id);
	if (door_id == 9) door_id = 0;
	if (it != doors.end()) {
		CheckExplodeNecessaryObject(door_id);
		doors.erase(it);
	}
}

void Screen::CheckExplodeNecessaryObject(int doorId) {
	if (isRealDoor(doorId)) { //explode real door
		finalMessage = "you blew up a necessary object for your progress. you lost the game!";
		gameState = false;
		Sleep(300);
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
			setChar(activeBombs[i].getPosition(), ' ');
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

string Screen::loadRiddles(){
	auto it = riddles.begin();
	int counter = 0;
	string path;
	for (auto& fileName: RiddlePathWays)
	{
		if (getRoomNumber(fileName) == currentRoom)
		{
			path = fileName;
			break;
		}
			
	}

	while (it != riddles.end()){
		string errorMsg = "";
		Riddle temp;
		if (currentRoom != roomIndex::VAULT)
			temp = ReadRiddleFromFile(path, it->first, counter, errorMsg);
		else
			temp = ReadVaultRiddleFromFile(path, it->first, errorMsg);

		if (errorMsg.empty()) {
			it->second = std::move(temp);
			++it;
			counter++;
		}
		else {
			return errorMsg;
		}
	}
	return "";
}

Riddle Screen::ReadRiddleFromFile(const string& filePath,const Point pos, int riddleIndex, string& errorMsg){
	std::ifstream inFile(filePath);
	std::string question;
	std::vector<std::string> options;
	int correctIndex = 0;

	if (!inFile.is_open()) {
		errorMsg = "Error: Could not open file [" + filePath + " ]";
		return Riddle();
	}
	string templine;
	for (int i = 0; i < riddleIndex; i++) { //skip to the right riddle index
		for (int j = 0; j < 6	; j++) { //every riddle is represnt by 6 lines in riddle text file
			if (!std::getline(inFile, templine)) {
				errorMsg = "Error: there is problem in [" + filePath + " ]";
				return Riddle();
			}
		}
	}

	for (int i = 0; i < 6; i++) {
		if (!std::getline(inFile, templine)) {
			errorMsg = "Error: there is problem in [" + filePath + " ]";
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

bool Screen::allRiddlesSolved() const{
	for (const auto& riddlePair : riddles) {
		if (!riddlePair.second.isSolved()) {
			return false;
		}
	}
	return true;
}
Riddle Screen::ReadVaultRiddleFromFile(const string& filePath, const Point pos, string& errorMsg) {
	std::ifstream inFile(filePath);
	
	std::string question;
	std::string correctAnswer;
	
	if (!inFile.is_open()) {
		errorMsg = "Error: Could not open file [" + filePath + " ]";
		return Riddle();
	}
	string templine;

	for (int i = 0; i < 2; i++) {
		if (!std::getline(inFile, templine)) {
			errorMsg = "Error: there is problem in [" + filePath + " ]";
			return Riddle();
		}
		if (!templine.empty() && templine.back() == '\r') {
			templine.pop_back(); // Remove carriage return character if present
		}
		if (i == 0) { question = templine; } //first line is the question
		else{ //i==1
			correctAnswer = templine ;//second line is the answer
		}
	}
	inFile.close();
	return Riddle(pos, question, correctAnswer);
}

void Screen::saveRoom()
{
	savedRooms[currentRoom].layout.clear();
	for (int i = 0; i < MAX_Y; i++) {
		savedRooms[currentRoom].layout.push_back(board[i]);
	}
	savedRooms[currentRoom].springs = springs;
	savedRooms[currentRoom].switches = switches;
	savedRooms[currentRoom].obstacles = obstacles;
	savedRooms[currentRoom].doors = doors;
	savedRooms[currentRoom].keys = keys;
	savedRooms[currentRoom].activeBombs = activeBombs;
	savedRooms[currentRoom].riddles = riddles;
	savedRooms[currentRoom].visited = true;
}