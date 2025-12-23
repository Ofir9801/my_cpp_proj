#pragma once
#include "Obstacle.h"
#include "Screen.h"
#include <vector>

void Obstacle::collectGroup(Point p, std::vector<Obstacle*>& group) {
	for (const auto& existing : group) {//if its already collected, no need to add again
		if (existing->getPosition() == p) return;
	}
	Obstacle* currentObj = board->getObstacleAt(p);
	if (currentObj) {
		group.push_back(currentObj);
	}
	else {
		return;
	}
	int dx[] = { 0, 0, 1, -1 };//checking all 4 directions - no diagonals
	int dy[] = { 1, -1, 0, 0 };
	for (int i = 0; i < 4; i++) {
		Point neighbor(p.getX() + dx[i], p.getY() + dy[i]);
		if (board->isValid(neighbor) && board->getCharAt(neighbor) == objSigns::OBSTACLE) {//if the neighbor is an obstacle, collect it too
			collectGroup(neighbor, group);
		}
	}
}

bool Obstacle::push(int force, Keyboard_bind dir) {
    std::vector<Obstacle*> group;
    collectGroup(this->position, group);
    if (force < group.size()) return false; // checking if weight allows push
    int dx = 0, dy = 0;
	switch (dir) {//determine direction of movement
    case UP:    dy = -1; break;
    case DOWN:  dy = 1;  break;
    case LEFT:  dx = -1; break;
    case RIGHT: dx = 1;  break;
    default: return false;
    }
    for (const auto* obs : group) {
        Point p = obs->getPosition();
        Point target(p.getX() + dx, p.getY() + dy);
        if (!board->isValid(target) || board->isWall(target)) return false;

        char targetChar = board->getCharAt(target);
        if (targetChar != ' ') {
            bool isSelfGroup = false;
            for (auto* internal : group) {
                if (internal->getPosition() == target) {
                    isSelfGroup = true;
                    break;
                }
            }
            if (!isSelfGroup) return false;
        }
    }
    for (auto* obs : group) {
        board->setChar(obs->getPosition(), ' ');
    }
    for (auto* obs : group) {
        Point p = obs->getPosition();
        Point newPos(p.getX() + dx, p.getY() + dy);

        obs->setPosition(newPos);
        board->setChar(newPos, objSigns::OBSTACLE);
    }
    return true;