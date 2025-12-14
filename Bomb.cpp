#include "Bomb.h"
#include "Screen.h"
#include "Player.h"
#include "Utils.h"
#include <algorithm> // for std::max, std::abs

void Bomb::explode(Screen& board, Player& p1, Player& p2) {
	active = false;
	board.showMessage("BOOM! The bomb exploded.");
	std::vector<Point> affectedPoints;//collecting first all the exploded tiles
	int bx = position.getX();
	int by = position.getY();
	for (int dy = -3; dy <= 3; dy++) {
		for (int dx = -3; dx <= 3; dx++) {
			Point target(bx + dx, by + dy);
			if (!board.isValid(target)) continue; //none board tiles
			if (isShielded(board, position, target)) continue;
			affectedPoints.push_back(target);
		}
	}
	for (const auto& p : affectedPoints) {
		char currentChar = board.getCharAt(p);
		if (currentChar == ' ' || currentChar == objSigns::OBSTACLE || board.isWall(p)) {//currently without player
			board.setChar(p, 'B');//for visual purposes
		}
	}
	Sleep(300);
	for (const auto& p : affectedPoints) {
		int distance = (std::max)(std::abs(p.getX() - bx), std::abs(p.getY() - by));
		destroyCell(board, p1, p2, p.getX(), p.getY(), distance);
	}
}

void Bomb::destroyCell(Screen& board, Player& p1, Player& p2, int x, int y, int distance) {
	Point target(x, y);
	if (board.isWall(target)) {
		if (distance <= 1) board.setChar(target, ' ');
		else board.setChar(target, '|');
		return;
	}
	if (p1.getPosition() == target) handlePlayerHit(p1);
	if (p2.getPosition() == target) handlePlayerHit(p2);
	if (p1.getPosition() != target && p2.getPosition() != target) {
		board.setChar(target, ' ');
	}

}
void Bomb::handlePlayerHit(Player& player) {
	Point dest = player.getPosition();
	player.decreaseLife();
	player.clearFromScreen();
	Sleep(500);
	player.reset(Point(dest.getX(), dest.getY(), player.getChar()));
}
bool Bomb::isShielded(Screen& board, Point bombPos, Point targetPos) {//bonus - handling walls. prototype so far
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
		Point p((int)round(currX), (int)round(currY));

		if (board.isWall(p)) return true; // true if a wall is found
	}
	return false;
}