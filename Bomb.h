#pragma once
#include "Point.h"
#include "Utils.h"
class Player; //forward declaration to avoid circular dependency
class Screen; //forward declaration to avoid circular dependency

class Bomb//Bomb class to represent bombs on the board
{//the first three fields are bomb's position, timer to explosion and active state
	Point position;
	int timer;
	bool active;
	void destroyCell(Screen& board, Player& p1, Player& p2, Point target); //helper function, self explanatory
	void handlePlayerHit(Player& player); //helper function, self explanatory
public:
	Bomb(const Point& pos) : position(pos), timer(EXPLODE_BOMB_TIME), active(true) {
		position.setChar((char)objSigns::BOMB);
	}
	void tick() { if (active) timer--;}
	bool shouldExplode() const { return active && timer <= 0; }
	Point getPosition() const { return position; }
	void explode(Screen& board,Player& p1, Player& p2);
	bool isShielded (Screen& board, Point bombPos, Point targetPos) const;
	int getTimer() const{ return timer; }
};


