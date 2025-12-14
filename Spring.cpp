#include "Spring.h"
#include "Screen.h"
#include <cmath>

int Spring::getSegmentIndex(const Point& p) const
{
        return std::abs(p.getX() - startPos.getX()) + std::abs(p.getY() - startPos.getY());
 }

bool Spring::isPlayerOn(const Point& p) const {
    if (pushDirection == Keyboard_bind::RIGHT || pushDirection == Keyboard_bind::LEFT) {//horizontal check
        if (p.getY() != startPos.getY()) return false;
        return (p.getX() >= startPos.getX() && p.getX() < startPos.getX() + length);
    }
    else {//vertical check
        if (p.getX() != startPos.getX()) return false;
        return (p.getY() >= startPos.getY() && p.getY() < startPos.getY() + length);
    }
}

bool Spring::isPointOn(const Point& p) const {
    if (p.getY() == startPos.getY()) {//horizontal check
        return (p.getX() >= startPos.getX() && p.getX() < startPos.getX() + length);
    }
    else {//vertical check
        return (p.getY() >= startPos.getY() && p.getY() < startPos.getY() + length);
    }
}


int Spring::calculateForce(const Point& playerPos) const {
    int distFromStart = 0;

    if (pushDirection == Keyboard_bind::RIGHT || pushDirection == Keyboard_bind::LEFT) {
        distFromStart = abs(playerPos.getX() - startPos.getX());
    }
    else {
        distFromStart = abs(playerPos.getY() - startPos.getY());
    }
    if (pushDirection == Keyboard_bind::RIGHT || pushDirection == Keyboard_bind::DOWN) {
        int force = length - distFromStart;
		return force > 0 ? force : 1;
    }
    else {
        return distFromStart + 1;
	}
}
void Spring::draw(const Point& playerPos,const Screen& board, bool active)const {
	//logic: iterate through each segment of the spring
	//deleting temporarily the spring character if the player is on/were on it
    if (board.isDark()) {
        return;
    }
    for (int i = 0; i < length; i++) {
        Point currentSegment = startPos;
        if (pushDirection == Keyboard_bind::RIGHT || pushDirection == Keyboard_bind::LEFT)
            currentSegment.setX(startPos.getX() + i);
        else
            currentSegment.setY(startPos.getY() + i);
        if (!active) {
            currentSegment.draw(objSigns::SPRING,board.IsColor() ? getColorForChar(objSigns::SPRING) : WHITE);
            continue;
        }
        if (currentSegment != playerPos) {
            currentSegment.draw(objSigns::SPRING, board.IsColor() ? getColorForChar(playerPos.getChar()) : WHITE );
        }
    }
}
