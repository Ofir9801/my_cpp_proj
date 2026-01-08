#include "Bomb.h"
#include "Screen.h"
#include "Player.h"
#include "Utils.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <algorithm> // for std::max, std::abs

void Bomb::explode(Screen& board, Player& p1, Player& p2) {
	active = false;
	board.showMessage("BOOM! The bomb exploded.");
	std::vector<Point> affectedPoints;//collecting first all the exploded tiles
	int bx = position.getX(); // bomb x
	int by = position.getY(); // bomb y
	for (int y = by-BOMB_RADIUS; y <= by + BOMB_RADIUS; y++) {
		for (int x = bx - BOMB_RADIUS; x <= bx + BOMB_RADIUS; x++) {
			Point target(x, y);
			if (!board.isValid(target)) {//if the explosion occurs on the game borders - skip
				continue;
			}
			if (!isShielded(board, position, target)) {
				char realCharAtBoard = board.getCharAt(target);
				target.setChar(realCharAtBoard);
				affectedPoints.push_back(target);
			}
		}
	}
	for (const auto& p : affectedPoints) {
		if (!board.IsSilent()) {
			board.setChar(p, 'B');//for visual purposes
		}
	}
	Sleep(300);
	for (const auto& p : affectedPoints) {
		destroyCell(board, p1, p2, p);
	}
}

void Bomb::destroyCell(Screen& board, Player& p1, Player& p2, Point target) {
	bool playerHit = false;
	
	if (p1.getPosition() == target) {
		handlePlayerHit(p1);
		playerHit = true;
	}
	if (p2.getPosition() == target) {
		handlePlayerHit(p2);
		playerHit = true;
	}
	if (!playerHit) {
		char c = target.getChar();
		char replaceChar = ' '; // Default replacement is empty space

		switch (static_cast<objSigns>(c)) {
		case objSigns::KEY:
			board.deleteKey(target);
			break;
		case objSigns::SPRING:
			board.deleteSpring(target);
			break;
		case objSigns::SWITCH_ON:
		case objSigns::SWITCH_OFF:
			board.deleteSwitch(target);
			break;
		case objSigns::OBSTACLE:
			board.deleteObstacle(target);
			break;
		case objSigns::RIDDLE:
			board.deleteRiddle(target);  
			break;
		default:
			if (isdigit(c)) {
				board.deleteDoor(target);
				replaceChar = 'X'; // Doors leave a specific mark
			}
			break;
		}
		if (board.isValid(target) || replaceChar == 'X') {
			board.setChar(target, replaceChar);
		}
	}
}

void Bomb::handlePlayerHit(Player& player) {
	Point newPos;
	player.decreaseLife();
	objSigns c = (objSigns)player.getChar();
	player.clearFromScreen();
	Sleep(300);
	switch (c) {
		case objSigns::PLAYER1:
			newPos.setX(PLAYER_1_START_X);
			newPos.setY(PLAYER_1_START_Y);
			newPos.setChar(objSigns::PLAYER1);
			player.reset(newPos);
			break;
		case objSigns::PLAYER2:
			newPos.setX(PLAYER_2_START_X);
			newPos.setY(PLAYER_2_START_Y);
			newPos.setChar(objSigns::PLAYER2);
			player.reset(newPos);
			break;
	}
}
bool Bomb::isShielded(Screen& board, Point bombPos, Point targetPos) const{//bonus - handling walls. prototype so far
	if (bombPos == targetPos) return false;
	int dx = targetPos.getX() - bombPos.getX();
	int dy = targetPos.getY() - bombPos.getY();
	int steps = (std::max)(std::abs(dx), std::abs(dy));
	float xStep = (float)dx / steps;
	float yStep = (float)dy / steps;
	//the float is to check diagonally as well
	float currX = (float)bombPos.getX();
	float currY = (float)bombPos.getY();
	for (int i = 1; i < steps; ++i) {
		currX += xStep;
		currY += yStep;
		Point p(static_cast<int>(round(currX)), static_cast<int>(round(currY)));

		if (board.isWall(p)) return true; // true if a wall is found
	}
	return false;
}

void Bomb::save(std::ofstream& file) const {
	position.save(file);
	file << timer << " ";
	file << (active ? 1 : 0) << "\n";
}

void Bomb::load(std::ifstream& file) {
	position.load(file);
	file.ignore();
	file >> timer;
	bool flag;
	file >> flag;
	active = (flag == 1);
}