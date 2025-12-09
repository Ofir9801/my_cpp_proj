#pragma once
#include "Point.h"
class Screen;//forward declaration to avoid circular dependency

class Spring {
    Point startPos;
    int length;
    Keys pushDirection;
    Screen& map;

public:
    Spring(Point start, int len, Keys dir, Screen& screen)//constractor
        : startPos(start), length(len), pushDirection(dir), map(screen) {}

    bool isPlayerOn(const Point& p) const;//function to state if the spring is being stepped
    int calculateForce(const Point& playerPos) const;//function to calculate the correct force granted
    Keys getDirection() const { return pushDirection; }
	void draw(const Point& playerPos, bool active = false) const;//function to draw the functioning of the spring on the map

};