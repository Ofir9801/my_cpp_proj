#include "Screen.h"
#include "Player.h"
#include "Utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include "Game.h"
#include "FileManager.h"
#include "LightingSystem.h"

using std::cout, std::endl;

Screen::Screen(unsigned int seed) : lighting(this) {
	attemptFunctionWithRetry([this]() {return this->loadAllRooms(); });
	currentRoom = static_cast<int>(roomIndex::MENU);//when start the game the first screen is menu
	if (seed == 0) {
		seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
	}
	setSeed(seed);
}

void Screen::setSeed(unsigned int seed) {
	currentSeed = seed;
	rng.seed(currentSeed);
}

void Screen::loadMap(int roomNumber, Point& doorPos){
	int lastRoom = currentRoom;
	currentRoom = roomNumber;
	const int legendY = getLegendY();

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
			char targetDoorChar;
			if (lastRoom == 0)
				targetDoorChar = '9';
			else
			targetDoorChar = static_cast<char>('0' + lastRoom); // e.g., if coming from Room 1, look for '1'
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
	if (roomDarkStatus.find(roomNumber)!=roomDarkStatus.end())
		lighting.setDarkRoom(roomDarkStatus[roomNumber]);
	else
		lighting.setDarkRoom(false);

	if (roomNumber == roomIndex::VICTORY && colorToggle) {
		if (!isSilent) { drawVictoryRoom();}
	}
	else {
		if(!isSilent){ drawMap(); }	
	}
}

void Screen::drawMap() {
	cls(); //clear the console
	const int legendY = getLegendY();
	if (lighting.IsDark()) {
		if (legendY != -1) {
			for (int i = 0; i < LEGEND_SIZE; i++) {
				gotoxy(0, legendY + i);
				cout << board[legendY + i];
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
		
	}
	string msg = "FINAL TEAM SCORE: " + std::to_string(sharedScore);
	int printCoordx = MAX_X / 2 -static_cast<int>( msg.size()) / 2;
	int printCoordy = MAX_Y - 5;
	gotoxy(printCoordx, printCoordy);
	cout << msg;
}

bool Screen::isWall(const Point& p) const {
	int legendY = getLegendY();

	if (inLegendBounds(legendY, p.getY())) {
		return true;
	}

	char c = getCharAt(p);
	return c == '-' || c == '|' || c == FAKE_DOOR_CHAR;
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
	if (isSilent) { return; }
	objSigns playerChar = (objSigns)p.getChar();
	string inventory = CreateInventoryDisplay(p);
	int legendY = getLegendY();
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
	cout << getLives() << std::flush;
	gotoxy(INFO_SLOTS::LIVES_START_X, legendY); //print lives
	cout << getScore() << std::flush;
}

void Screen::setChar(const Point& p, char c) {
	if (!p.InBounds())
		return;
	int legendY = getLegendY();
	if (inLegendBounds(legendY, p.getY())) {
		return;
	}
	board[p.getY()][p.getX()] = c;
	if (isSilent) return;
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

void Screen::showInstructionBinds() const{
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
void Screen::showMessage(string msg) {
	if (isSilent) { return; }

	gotoxy(MESSAGES_POS::MES_X, roomLegendRows[currentRoom] + MESSAGES_POS::MES_Y);
	string extraSpace(MAX_X - msg.length(), ' ');
	cout << msg <<extraSpace <<std::flush;
	MessageTimer = TIMER_MESSAGE;
}

void Screen::attemptFunctionWithRetry(std::function<string()> func) {
	while (true) {
		string errorMsg = func();
		if (errorMsg.empty()) return;

		cls();
		cout << "########################################################" << std::endl;
		cout << "ERROR LOADING Game" << endl;
		cout << "########################################################" << std::endl;
		cout << errorMsg << endl;
		cout << "1. Fix the file externally." << endl;
		cout << "2. Press 'r' to RETRY." << endl;
		cout << "3. Press 'ESC' to EXIT Game." << endl;
		char c = static_cast<char>(_getch());
		if (c == ESC) {
			throw std::runtime_error("Game stopped by user due to file error");
		}
	}
}

string Screen::loadAllRooms() {
	vector<string> roomFilePaths;
	getAllFilePaths(roomFilePaths, ROOM_EXTENSION, ROOM_FOLDER);
	
	if (roomFilePaths.empty())
		return "Error: No .screen files in " + ROOM_FOLDER;

	for (const auto& fullPath : roomFilePaths) {
		//האם צריך פה try catch?
		try{
			int roomNum = getRoomNumber(fullPath);
			bool requiresLegend = isGameRoom(roomNum);
			RoomData data = ReadRoomFromFile(fullPath, requiresLegend);
			if (!data.errorMsg.empty()) {
				return data.errorMsg;
			}

			Rooms[roomNum] = data.layout;
			roomLegendRows[roomNum] = data.legendLoc;
			roomDarkStatus[roomNum] = data.isDark;
		}
		catch (const std::exception& e) {
			return "Error processing [" + fullPath + "]: " + e.what();
		}
	}
	return "";
}

string Screen::loadRiddlesForCurrentRoom() {
	if (riddles.empty()) {
		return "";
	}
	vector<string> riddleFilePaths;
	getAllFilePaths(riddleFilePaths, RIDDLES_EXTENSION, RIDDLES_FOLDER);
	string targetPath = "";
	bool found = false;

	for (const auto& path : riddleFilePaths) {
		try {
			if (static_cast<int>(getRoomNumber(path)) == currentRoom) {
				targetPath = path;
				found = true;
				break;
			}
		}
		catch (...) {
			continue;
		}
	}
	if (!found) {
		return "Error: No .riddle files for room " + std::to_string(currentRoom) + " in " + RIDDLES_FOLDER;
	}
	int riddleCounter = 0;
	for (auto& riddlePair : riddles) {
		string errorMsg = "";
		Riddle temp;
		if(currentRoom != static_cast<int>(roomIndex::VAULT))
			temp = ReadRiddleFromFile(targetPath,riddlePair.first,riddleCounter, errorMsg);
		else
			temp = ReadVaultRiddleFromFile(targetPath,riddlePair.first, errorMsg); //vault riddle has no position index
		
		if (!errorMsg.empty()) {
			return errorMsg;
		}
		riddlePair.second = std::move(temp);
		riddleCounter++;
	}
	return "";
}

Obstacle* Screen::getObstacleAt(const Point& p) {
	auto it = obstacles.find(p);
	if (it != obstacles.end())
		return &it->second;
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
			if (!isSilent) {
				s.draw(Point(0, 0), *this, false);
			}
		}
	}
}

void Screen::clearMessegeArea(bool forceClean){
	if (forceClean) {
		gotoxy(MESSAGES_POS::MES_X, roomLegendRows[currentRoom] + MESSAGES_POS::MES_Y);
		cout << EMPTYLINE << std::flush;
	}
	else {
		if (MessageTimer > 0) {
			MessageTimer--;
			if (MessageTimer == 0) {
				gotoxy(MESSAGES_POS::MES_X, roomLegendRows[currentRoom] + MESSAGES_POS::MES_Y);
				cout << EMPTYLINE << std::flush;
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

	int legendY = getLegendY();
	for (int y = 0; y < MAX_Y; y++) { 
		if (inLegendBounds(legendY,y)) { //skip legend lines from scanning
			continue;
		}
		for (int x = 0; x < MAX_X; x++) {
			Point p(x, y);
			char c = getCharAt(p);
			
			if (c == objSigns::SWITCH_OFF) {
				switches.push_back(Switch(x, y, false));
			}
			else if (c == objSigns::SWITCH_ON) {
				switches.push_back(Switch(x, y, true));
			}
			else if (c == objSigns::OBSTACLE) {
				obstacles[Point(x, y)] = Obstacle(x, y, this);
			}
			else if (c == objSigns::KEY) {
				keys[Point(x, y)] = Key(x, y);
			}
			else if (c == objSigns::RIDDLE) {
				riddles[Point(x, y)] = Riddle(Point(x, y, objSigns::RIDDLE));
			}
			else if (isdigit(static_cast<unsigned char>(c))) {
				int door_id = normalizeDoorId(c-'0');
				doors[door_id] = Door(x, y, c);
				if (door_id == doorIdOpen) {
					doors[door_id].open();
					doorPos = p;
				}
				else {
					doorIDs.push_back(door_id);
				}
			}
			
		}
	}
	linkDoorsToKeysAndSwitches();
	attemptFunctionWithRetry([this]() {return this->loadRiddlesForCurrentRoom(); });
	loadSprings();
}

void Screen::linkDoorsToKeysAndSwitches() { //the assumption is that the number of switches and  is equal to the number of doors
	vector <int> doorIdCopy = doorIDs;  //make copy of doorIds vector 

	int currentIndex = 0;
	std::shuffle(doorIdCopy.begin(), doorIdCopy.end(), rng);
	auto it = keys.begin();
	while (it != keys.end() && currentIndex < doorIdCopy.size()) {
			int currentDoorId = doorIdCopy[currentIndex];
			it->second.setTargetDoorId(currentDoorId);
			++it;
			currentIndex++;
	}
	
	std::shuffle(doorIdCopy.begin(), doorIdCopy.end(), rng);
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

void Screen::openDoor(int door_id, char player){
	auto it = doors.find(door_id);
	if (it != doors.end()) {
		it->second.open();
		showMessage("Door opened!");
		if (game) {
			game->onGameEvent(Event(game->getIteration(), EventType::DOOR_OPEN, player, "door opened"));
		}
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

void Screen::handleSwitches(const Point& p1, const Point& p2)
{
	for (auto& s : switches) {
		bool isPresed = s.isAt(p1) || s.isAt(p2);

		if (s.update(isPresed)) {
			Point pos = s.getPosition();
			setChar(pos, pos.getChar());
			showMessage(s.getIsOn() ? "Switch is ON" : "Switch is OFF");
		}
	}
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
	string answer = "";
	if (it != riddles.end() && !it->second.isSolved()) {
		bool solved = riddles[riddlePos].engage(p, answer);
		drawMap(); //redraw the map after riddle engagement
		if (solved) {
			string msg = "Correct! + " + std::to_string(SUCCESS_SCORE) + " points!";
			showMessage(msg);
			if (game) {
				game->onGameEvent(Event(game->getIteration(), EventType::RIDDLE_SOLVED, p.getChar(),answer + " - Correct Answer "));
			}
			return true;
		}
		else {
			string msg = "WRONG! You lost " + std::to_string(WRONG_ANSWER) + " points!";
			showMessage(msg);
			if (game) {
				game->onGameEvent(Event(game->getIteration(), EventType::RIDDLE_WRONG_ANSWER, p.getChar(),answer + " - Wrong Answer "));
			}
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
	string answer = "";
	if (it != riddles.end() && !it->second.isSolved()) {
		bool solved = riddles[riddlePos].engageVaultRiddle(answer);
		drawMap(); //redraw the map after riddle engagement
		if (solved) {
			string msg = "Congratulations! You solved the last challenge. Proceed to the final door";
			setChar(riddlePos, objSigns::EMPTY);
			showMessage(msg);
			if (game) {
				game->onGameEvent(Event(game->getIteration(), EventType::RIDDLE_SOLVED, ' ', answer + " - Correct Answer "));
			}
			return true;
		}
		else {
			string msg = "WRONG! Check your answers and come back again";
			showMessage(msg);
			if (game) {
				game->onGameEvent(Event(game->getIteration(), EventType::RIDDLE_WRONG_ANSWER, ' ', answer + " - Wrong Answer "));
			}
			return false;
		}
	}
	else if (it != riddles.end()) {
		string msg = "You Already solved this riddle, proceed to the final door";
		showMessage(msg);
	}
	return false;
}

bool Screen::BoxDistance(int x, int y, const Point& p, int radius) const {
	int dx =std::abs(x - p.getX()); //calculate the distance between two points by x
	int dy = std::abs(y - p.getY()); //calculate the distance between two points by y
	return dx <= radius && dy <= radius;
	//return (dx * dx + dy * dy) <= (radius * radius); //true if the given point is in player's light radius
}

int Screen::getLegendY() const
{
	auto it = roomLegendRows.find(currentRoom);
	if (it != roomLegendRows.end()) {
		return it->second;
	}
	return -1;
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
	int legendY = getLegendY();
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

void Screen::deleteRiddle(Point position)
{
	auto it = riddles.find(position);
	if (it != riddles.end()) {
		riddles.erase(it);
		showMessage("you maybe need that. think about restart the game");
	}
}

void Screen::deleteSwitch(Point position){
	bool flag = false;
	vector<Switch>::iterator it = switches.begin();
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

void Screen::deleteObstacle(Point position)
{
	auto it = obstacles.find(position);
	if (it != obstacles.end()) {
		obstacles.erase(it);
	}
}

bool Screen::isSameObstacleGroup(Point p1, Point p2){
	Obstacle* obs1 = getObstacleAt(p1);
	Obstacle* obs2 = getObstacleAt(p2);

	//if one of the obstacles doesn't exist
	if (!obs1 || !obs2) return false;

	// if both points are the same
	if (p1 == p2) return true;

	// use the first obstacle to collect its group
	std::vector<Obstacle*> group;
	obs1->collectGroup(p1, group);

	//chcek if the second obstacle is in the same group
	for (const auto* obs : group) {
		if (obs->getPosition() == p2) {
			return true;
		}
	}
	return false;
}

int Screen::getAssistForce(Point obstaclePos, Keyboard_bind pushDir, const Player* whoIsAsking) {
	//identify the partner player
	Player* partner = (whoIsAsking == player1) ? player2 : player1;
	if (!partner) return 0;

	//chcek if the partner is pushing in the same direction
	if (partner->getDirection() != pushDir) return 0;

	//get the partner position
	Point partnerPos = partner->getPosition();
	Point partnerTargetPos = partnerPos;
	partnerTargetPos.setDirection(pushDir);
	partnerTargetPos.move();
	
	//check if the target position is the same obstacle group as the original obstacle
	if (isSameObstacleGroup(obstaclePos, partnerTargetPos)) {
		//return the partner's force
		return partner->getForce();
	}
	return 0;
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
		setChar(activeBombs[i].getPosition(),static_cast<char>( '0' + time / 10));
		if (activeBombs[i].shouldExplode()) {
			setChar(activeBombs[i].getPosition(), ' ');
			activeBombs[i].explode(*this, p1, p2);
			setChar(activeBombs[i].getPosition(), ' ');
			activeBombs.erase(activeBombs.begin() + i);
			if (game) {
				game->onGameEvent(Event(game->getIteration(), EventType::BOMB_EXPLODE, ' ', "Bomb had exploded"));
			}
		}
		else {
			++i;
		}
	}
}
void Screen::decreaseLife() {
	if (sharedLives > 0) {
		sharedLives--;
		if (game) {
			game->onGameEvent(Event(game->getIteration(), EventType::LIFE_LOST, ' ', "Life lost. Lives: " + std::to_string(sharedLives)));
		}
		showMessage("Life lost, be carefull! Lives left: " + std::to_string(sharedLives));
	}
	else
	{
		gameState = false;
		showMessage("No lives left! Game Over!");
		cls();
	}
}

Riddle Screen::ReadRiddleFromFile(const string& filePath,const Point pos, int riddleIndex, string& errorMsg){
	std::ifstream inFile(filePath);
	string question;
	vector<string> options;
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
	
	string question;
	string correctAnswer;
	
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

string Screen::selectSavedFile()
{
	auto saves = FileManager::getSavedGamesList();
	if (saves.empty()) {
		return "";
	}

	cls();
	cout << "Select a Saved Game:\n";
	cout << "--------------------\n";

	for (size_t i = 0; i < saves.size() && i < MAX_SAVED_SLOTS; ++i) {
		string line = (i == MAX_SAVED_SLOTS - 1) ? "0." : std::to_string(i + 1) + ". ";
		cout << line << saves[i].displayName << "  (" << saves[i].timestampStr << ")\n";
	}
	cout << "ESC. Cancel\n";

	while (true) {
		if (_kbhit()) {
			char key = static_cast<char>(_getch());
			if (key == ESC) return "";
			if (isdigit(key)) {
				size_t index = (key == '0') ? 9 : (key - '1');
				if (index < saves.size()) {
					return saves[index].folderName;
				}
			}
		}
	}
}

void Screen::saveGame()
{
	currentSaveDirectory = FileManager::saveGame(savedRooms, sharedLives, sharedScore, currentRoom, colorToggle, currentSaveDirectory);
}

int Screen::loadGame(const string& folderPath)
{
	currentSaveDirectory = folderPath;
	savedRooms.clear();
	//check this
	attemptFunctionWithRetry([this, folderPath]() {return FileManager::loadGame(savedRooms, sharedLives, sharedScore, folderPath, currentRoom, colorToggle, this);});
	return currentRoom;
}


void Screen::cleanSavedGames()
{
	system("cls");
	cout << "!!! WARNING !!!\n";
	cout << "You are about to delete ALL saved games.\n";
	cout << "This action cannot be undone.\n\n";
	cout << "Are you sure? (y/n): ";

	char confirm =static_cast<char>( _getch());
	if (tolower(confirm) == 'y') {

		try {
			FileManager::cleanSavedGames();
			cout << "\n\nAll saved games deleted successfully.\n";
		}
		catch (const std::exception& e) {
			cout << "\n\nError deleting files: " << e.what() << "\n";
		}
	}
	else {
		cout << "\nOperation Cancelled.\n";
		Sleep(500);
	}
}


