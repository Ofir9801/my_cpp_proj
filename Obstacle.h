#pragma once
#include "Point.h"
#include <vector>
class Screen;//forward declaration to avoid circular dependency
class Obstacle
{
	Point position;
	int weight;
	Screen* board;
	void collectGroup(Point p, std::vector<Obstacle*>& group);// recursive function to gather connected obstacles
public:
	Obstacle(int x, int y, Screen* theMap, int weight = 1): position(x, y, '*'), board(theMap), weight(weight) {}
	bool push(int force, Keyboard_bind dir);
	Point getPosition() const { return position; }
	void setPosition(const Point& p) { position = p; }
};

