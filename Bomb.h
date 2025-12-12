#pragma once
#include "Point.h"
#include "Utils.h"
class Player; //forward declaration to avoid circular dependency
class Screen; //forward declaration to avoid circular dependency

class Bomb
{
	Point position;
	int timer;
	bool active;
	void destroyCell(Screen& map, Player& p1, Player& p2, int x, int y, int distance); //helper function, self explanatory
	void handlePlayerHit(Player& player); //helper function, self explanatory
public:
	Bomb(const Point& pos) : position(pos), timer(10000), active(true) {
		position.setChar(objSigns::BOMB);
	}
	void tick() { if (active) timer--;}
	bool shouldExplode() const { return active && timer <= 0; }
	Point getPosition() const { return position; }
	void explode(Screen& map,Player& p1, Player& p2);
	bool isShielded(Screen& map, Point bombPos, Point targetPos);
};

