#include "Spring.h"
#include "Screen.h"
#include <cmath>

bool Spring::isPlayerOn(const Point& p) const {
    if (pushDirection == Keys::RIGHT || pushDirection == Keys::LEFT) {//horizontal check
        if (p.getY() != startPos.getY()) return false;
        return (p.getX() >= startPos.getX() && p.getX() < startPos.getX() + length);
    }
    else {//vertical check
        if (p.getX() != startPos.getX()) return false;
        return (p.getY() >= startPos.getY() && p.getY() < startPos.getY() + length);
    }
}

int Spring::calculateForce(const Point& playerPos) const {
    int distFromStart = 0;

    if (pushDirection == Keys::RIGHT || pushDirection == Keys::LEFT) {
        distFromStart = abs(playerPos.getX() - startPos.getX());
    }
    else {
        distFromStart = abs(playerPos.getY() - startPos.getY());
    }
    if (pushDirection == Keys::RIGHT || pushDirection == Keys::DOWN) {
        int force = length - distFromStart;
		return force > 0 ? force : 1;
    }
    else {
        return distFromStart + 1;
	}
}
void Spring::draw(const Point& playerPos, bool active) const{
	//logic: iterate through each segment of the spring
	//deleting temporarily the spring character if the player is on/were on it
    for (int i = 0; i < length; i++) {
        Point currentSegment = startPos;
        if (pushDirection == Keys::RIGHT || pushDirection == Keys::LEFT)
            currentSegment.setX(startPos.getX() + i);
        else
            currentSegment.setY(startPos.getY() + i);
        if (!active) {
            currentSegment.draw(objSigns::SPRING,map.IsColor() ? getColorForChar(objSigns::SPRING) : WHITE);
            continue;
        }
        if (currentSegment == playerPos) {
        }
        else {
            currentSegment.draw(objSigns::SPRING, getColorForChar(playerPos.getChar()));
           // currentSegment.draw(objSigns::SPRING,  BLACK);
        }
    }
}
