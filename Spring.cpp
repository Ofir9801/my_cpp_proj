#include "Spring.h"
#include "Screen.h"
#include <cmath>
#include <ostream>
#include <fstream>

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
		if (board.IsSilent()) continue;
        if (!active) {
            currentSegment.draw(objSigns::SPRING,board.IsColor() ? getColorForChar(objSigns::SPRING) : Color::WHITE);
            continue;
        }
        if (currentSegment != playerPos) {
            currentSegment.draw(objSigns::SPRING, board.IsColor() ? getColorForChar(playerPos.getChar()) : Color::WHITE );
        }
    }
}

void Spring::interact(Player& p, const Screen& board){
    draw(p.getPosition(), board, true);
    Point checkWall = p.getPosition();
    checkWall.move();
	bool hittingWall = board.isWall(checkWall);
	bool isStaying = (p.getPosition().getDirectionEnum() == Keyboard_bind::STAY);

    if (hittingWall || isStaying) {
        Sleep(200); //pause to show the spring effect
        int force = calculateForce(p.getPosition());
        p.SetSpringState(force, pushDirection);
	}
}

bool Spring::handleExplosion(const Point& hitPos, Screen& board)
{
    int hitIndex = getSegmentIndex(hitPos);
	
	for (int i = hitIndex; i < length; i++) { //remove the segments from the hit index to the end
		Point currentSegment = startPos;
		if (pushDirection == Keyboard_bind::RIGHT || pushDirection == Keyboard_bind::LEFT)
			currentSegment.setX(startPos.getX() + i);
		else
            currentSegment.setY(startPos.getY() + i);
		board.setChar(currentSegment, objSigns::EMPTY);
    }
    length = hitIndex; // Shorten the spring
    return (length <= 0);
}

Spring* Spring::CreateFromMap(const Screen& board, int x, int y, bool(&processed)[MAX_Y][MAX_X])
{
    bool isHorizontal = (x + 1 < MAX_X && board.getCharAt(Point(x + 1, y)) == objSigns::SPRING);
    if (!isHorizontal && (y + 1 >= MAX_Y || board.getCharAt(Point(x, y + 1)) != objSigns::SPRING)) {
        isHorizontal = true; //default to horizontal if single spring char
    }
    int length = 0;
    Keyboard_bind dir = Keyboard_bind::STAY;
    Point currentStart(x, y);

    if (isHorizontal) {
        int currX = x;
        while (currX < MAX_X && board.getCharAt(Point(currX, y)) == objSigns::SPRING) {
            processed[y][currX] = true;
            length++;
            currX++;
        }
        if(board.isWall(Point(x-1,y))){dir = Keyboard_bind::RIGHT; }
        else if (board.isWall(Point(x + length, y))) dir = Keyboard_bind::LEFT;
        else if (board.isWall(Point(x, y - 1))) dir = Keyboard_bind::DOWN;
		else if (board.isWall(Point(x, y + 1))) dir = Keyboard_bind::UP;
    }
    else {
        int currY = y;
        while (currY < MAX_Y && board.getCharAt(Point(x, currY)) == objSigns::SPRING) {
            processed[currY][x] = true;
            length++;
            currY++;
        }
        // Check walls for direction
        if (board.isWall(Point(x, y - 1))) dir = Keyboard_bind::DOWN;
        else if (board.isWall(Point(x, y + length))) dir = Keyboard_bind::UP;
    }
    if (dir != Keyboard_bind::STAY) {
        return new Spring(currentStart, length, dir);
    }
    return nullptr;
}

void Spring::save(std::ofstream& file) const
{
    startPos.save(file);
    file << length << "\n";
    file << static_cast<int>(pushDirection) << "\n";
}

void Spring::load(std::ifstream& file)
{
    startPos.load(file);
    file.ignore();
    file >> length;
    int dir;
    file >> dir;
    pushDirection = static_cast<Keyboard_bind>(dir);
}
