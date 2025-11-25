#pragma once
#include "Screen.h"
#include "Point.h"
class Obstacle
{
	point position;
	int weight;
	Screen& map;
public:
	Obstacle(int x, int y, Screen& theMap, int weight = 1): position(x, y, '#'), map(theMap), weight(weight) {
	}

	void push(int force, Keys dir);
	
};

