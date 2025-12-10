#pragma once
#include "Point.h"
class Screen; //forward declaration to avoid circular dependency

class Bomb
{
	Point position;
	int timer;
	bool active;
	void destroyCell(Screen& map, int x, int y); //helper function, self explanatory
public:
	Bomb(const Point& pos) : position(pos), timer(5), active(true) {
		position.setChar('@');
	}
	void tick() { if (active) timer--;}
	bool shouldExplode() const { return timer <= 0; }
	Point getPosition() const { return position; }
	void explode(Screen& map);
};

