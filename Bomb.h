#pragma once
#include "point.h"
class Screen; //forward declaration to avoid circular dependency

class Bomb
{
	point position;
	int timer;
	bool active;
	void destroyCell(Screen& map, int x, int y); //helper function, self explanatory
public:
	Bomb(const point& pos) : position(pos), timer(5), active(true) {
		position.setChar('@');
	}
	void tick() { if (active) timer--;}
	bool shouldExplode() const { return timer <= 0; }
	point getPosition() const { return position; }
	void explode(Screen& map);
};

