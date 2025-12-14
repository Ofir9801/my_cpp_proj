#pragma once
#include "Point.h"
class Screen;//forward declaration to avoid circular dependency
class Obstacle
{
	Point position;
	int weight;
	Screen* board;
public:
	Obstacle(int x, int y, Screen* theMap, int weight = 1): position(x, y, '*'), board(theMap), weight(weight) {}

	void push(int force, Keyboard_bind dir);
	
	
};

