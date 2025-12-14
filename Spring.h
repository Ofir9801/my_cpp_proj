#pragma once
#include "Point.h"
class Screen;//forward declaration to avoid circular dependency

class Spring {
    Point startPos;
    int length;
    Keyboard_bind pushDirection;
    //Screen& board;

public:
    Spring(Point start, int len, Keyboard_bind dir)//constractor
        : startPos(start), length(len), pushDirection(dir){ }
    int getLength() const { return length; };
    void setLength(int l) { length = l; };
    Point getPosition() const { return startPos; };
    int getSegmentIndex(const Point& p) const;
    bool isPlayerOn(const Point& p) const;//function to state if the spring is being stepped
    bool isPointOn(const Point& p) const;
    int calculateForce(const Point& playerPos) const;//function to calculate the correct force granted
    Keyboard_bind getDirection() const { return pushDirection; }
	void draw(const Point& playerPos, const Screen& board ,bool active = false)const ;//function to draw the functioning of the spring on the board

};