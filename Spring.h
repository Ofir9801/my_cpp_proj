#pragma once
#include "Point.h"
class Screen;//forward declaration to avoid circular dependency
class Player;//forward declaration to avoid circular dependency

class Spring {//Spring class to represent springs on the board
    //Spring's fields are starting position, length and direction.
    Point startPos;
    int length;//to determine speed
    Keyboard_bind pushDirection;//each springbar has a single direction

public:
    Spring(Point start, int len, Keyboard_bind dir)//constractor
        : startPos(start), length(len), pushDirection(dir){ }
    //int getLength() const { return length; };
   // void setLength(int l) { length = l; };
    //Point getPosition() const { return startPos; };
    bool isPlayerOn(const Point& p) const;//function to state if the spring is being stepped
    //Keyboard_bind getDirection() const { return pushDirection; }
	void draw(const Point& playerPos, const Screen& board ,bool active = false)const ;//function to draw the functioning of the spring on the board
    void interact(Player& p, const Screen& board);
    bool handleExplosion(const Point& hitPos,Screen& board);
    bool isPointOn(const Point& p) const;
    static Spring* CreateFromMap(const Screen& board, int x, int y, bool(&processed)[MAX_Y][MAX_X]);
private:
    int getSegmentIndex(const Point& p) const;
    int calculateForce(const Point& playerPos) const;//function to calculate the correct force granted
};