#include "Bomb.h"
#include "Player.h"
#include "Riddle.h"
#include "Screen.h"
#include "Spring.h"
#include "Utils.h"
#include <algorithm>
#include <cctype> //  for tolower, isdigit
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <windows.h>

using std::cout;
using std::endl;
using std::string;

Screen::Screen() {
	initaializeRoomsArray();
	for (int i = 0; i < MAX_Y; i++) {
		board[i].resize(MAX_X, ' ');
	}
}

void Screen::loadMap(int roomNumber){
	int lastRoom = currentRoom;
	currentRoom = roomNumber;
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
	}
	else { //first time loading the room
		for (int i = 0; i < MAX_Y; i++) {
			board[i] = Rooms[roomNumber][i];
		}
		if (lastRoom != roomIndex::MENU) {
			loadItems(lastRoom);
		}
		else {
			loadItems(-1); //no door to open
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
	if (isDarkRoom) {
		for (int i = 0; i < 2; i++) {cout << board[i];}
	}
	else {
		for (int i = 0; i < MAX_Y; i++) {
			gotoxy(0, i);
			if (i > 2 && colorToggle) {
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
	}
}

bool Screen::isWall(const Point& p) const{
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
	objSigns playerChar =(objSigns) p.getChar();
	string inventory = CreateInventoryDisplay(p);

	switch (playerChar) {
	case objSigns::PLAYER1:
		gotoxy(INFO_SLOTS::PLAYER1_SIGN_START_X, INFO_SLOTS::PLAYER_SIGN_Y); //print player char
		cout << playerChar << std::flush;
		gotoxy(INFO_SLOTS::PLAYER1_INV_START_X, INFO_SLOTS::PLAYER_INV_Y); //print inventory
		cout << inventory << std::flush;
		break;
	case objSigns::PLAYER2:
		gotoxy(INFO_SLOTS::PLAYER2_SIGN_START_X, INFO_SLOTS::PLAYER_SIGN_Y); //print player char
		cout << playerChar << std::flush;
		gotoxy(INFO_SLOTS::PLAYER2_INV_START_X, INFO_SLOTS::PLAYER_INV_Y);//print inventory
		cout << inventory << std::flush;
		break;
	}
	gotoxy(INFO_SLOTS::SCORE_START_X, INFO_SLOTS::PLAYER_SIGN_Y); //print score
	cout << p.getLives() << std::flush;
	gotoxy(INFO_SLOTS::LIVES_START_X, INFO_SLOTS::PLAYER_SIGN_Y); //print lives
	cout << p.getScore() << std::flush;
}

void Screen::setChar(const Point& p, char c) {
	if (!p.InBounds())
		return;
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
	gotoxy(MESSAGES_POS::MES_X, MESSAGES_POS::MES_Y);
	string extraSpace(MAX_X - msg.length(), ' ');
	std::cout << msg <<extraSpace <<std::flush;
	MessageTimer = TIMER_MESSAGE;
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
	if (ReadRoomLayoutFromFile(VaultPathWay, roomIndex::VAULT)) {
		throw std::runtime_error("Something wrong with the file vault.txt");
	}
	if (ReadRoomLayoutFromFile(EndingScreenPathWay, roomIndex::VICTORY)){
		throw std::runtime_error("Something wrong with the file endingscreen.txt"); 
	}	
	Rooms[(int)roomIndex::MENU] = Menu;
	Rooms[(int)roomIndex::INSTRUCTIONS] = Instructions;
	Rooms[(int)roomIndex::ROOM1] = Room1;
	Rooms[(int)roomIndex::ROOM2] = Room2;
	Rooms[(int)roomIndex::ROOM3] = Room3;
	Rooms[(int)roomIndex::VAULT] = Vault;
	Rooms[(int)roomIndex::VICTORY] = EndingScreen;
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

void Screen::clearMessegeArea(){
	if (MessageTimer > 0) {
		MessageTimer--;
		if (MessageTimer == 0) {
			gotoxy(MESSAGES_POS::MES_X, MESSAGES_POS::MES_Y);
			std::cout << EMPTYLINE<<std::flush;
		}
	}
}

void Screen::loadItems(int doorIdOpen) {//enter the items from the board to the appropiete data structures
	switches.clear();
	obstacles.clear();
	doors.clear();
	doorIDs.clear();
	keys.clear();
	riddles.clear();
	for (int y = 3; y < MAX_Y; y++) {
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
			else if (isdigit((unsigned char)c)) {
				int door_id = c - '0';
				doors[door_id] = Door(x, y, c);
				if (door_id == doorIdOpen) {
					doors[door_id].open();
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
	loadRiddles();
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
		setconnection(currentDoorId);
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

void Screen::setconnection(int door_id) {
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
	int dx =std::abs(x - p.getX()); //calculate the the distance between two points by x
	int dy = std::abs(y - p.getY()); //calculate the the distance between two points by y
	return dx <= radius && dy <= radius;
	//return (dx * dx + dy * dy) <= (radius * radius); //true if the given point is in player's light radius
}
bool Screen::RoundDistance(int x, int y, const Point& p, int radius) const {
	int dx = x - p.getX(); //calculate the the distance between two points by x
	int dy = y - p.getY(); //calculate the the distance between two points by y
	return (dx * dx + dy * dy) <= (radius * radius); //true if the given point is in player's light radius
}


void Screen::ProcessLightning(int cx,int cy, int radius, bool erase, const Point& p1,const Point& p2, const int r1, const int r2) {
	for (int y = cy - radius; y <= cy + radius; y++) {
		for (int x = cx - radius; x <= cx + radius; x++) {
			if (x < 0 || x >= MAX_X || y < 3 || y >= MAX_Y)
				continue;

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
	bool insideBoard = p.InBounds(MAX_X - 1, MAX_Y - 1, 1, 4);
	return insideBoard;
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
	if (it != doors.end()) {
		CheckExplodeNecessaryObject(door_id);
		doors.erase(it);
	}
}

void Screen::CheckExplodeNecessaryObject(int doorId) {
	if (isRealDoor(doorId)) { //explode real door
		gameState = false;
		showMessage("you blew up a necessary object for your progress. you lost the game!");
		Sleep(2000);
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
		switch ((roomIndex)currentRoom) {
			case roomIndex::ROOM1:
				temp = ReadRiddleFromFile(RiddlesRoom1PathWay,it->first, counter, error);
			break;
			case roomIndex::ROOM2:
				temp = ReadRiddleFromFile(RiddlesRoom2PathWay, it->first, counter, error);
				break;
			case roomIndex::ROOM3:
				temp = ReadRiddleFromFile(RiddlesRoom3PathWay, it->first, counter, error);
				break;
			case roomIndex::VAULT:
				temp = ReadVaultRiddleFromFile(RiddlesVaultPathWay, it->first, error);
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

bool Screen::allRiddlesSolved() const{
	for (const auto& riddlePair : riddles) {
		if (!riddlePair.second.isSolved()) {
			return false;
		}
	}
	return true;
}
Riddle Screen::ReadVaultRiddleFromFile(const string& filePath, const Point pos, bool& error) {
	std::ifstream inFile(filePath);
	if (!inFile.is_open()) {
		std::cout << "DEBUG: Failed to open: [" << filePath << "]" << std::endl;
		// Check if the file even exists according to the OS
		std::ifstream test(filePath.c_str());
		if (!test) perror("System Error Message");
	}
	std::string question;
	std::string correctAnswer;
	
	if (!inFile.is_open()) {
		error = true;
		return Riddle();
	}
	string templine;

	for (int i = 0; i < 2; i++) {
		if (!std::getline(inFile, templine)) {
			error = true;
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



