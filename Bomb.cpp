#include "Bomb.h"
#include "Screen.h"
#include "Player.h"
#include "Utils.h"
#include <windows.h>
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
		board.setChar(p, 'B');//for visual purposes

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
	if (!playerHit){
		char c = target.getChar();
		if (c == objSigns::KEY) { 
			board.deleteKey(target);
			board.setChar(target, ' ');
		}
		else if (c == objSigns::SPRING) {
			board.deleteSpring(target);
			board.setChar(target, ' ');
		}
		else if (c == objSigns::SWITCH_ON || c == objSigns::SWITCH_OFF) {
			board.deleteSwitch(target);
			board.setChar(target, ' ');
		}
		else if (isdigit(c)){
			board.deleteDoor(target); 
			board.setChar(target, 'X');
		}
		else{ board.setChar(target, ' '); }
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