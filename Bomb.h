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
public:
	Bomb(const Point& pos) : position(pos), timer(EXPLODE_BOMB_TIME), active(true) {
		position.setChar(objSigns::BOMB);
	}
	Bomb() :position(), timer(-1), active(false){}
	void tick() { if (active) timer--;}
	bool shouldExplode() const { return active && timer <= 0; }
	Point getPosition() const { return position; }
	void explode(Screen& board,Player& p1, Player& p2);
	int getTimer() const{ return timer; }
	void save(std::ofstream& file) const;
	void load(std::ifstream& file);

private:
	void destroyCell(Screen& board, Player& p1, Player& p2, Point target); //helper function, self explanatory
	void handlePlayerHit(Player& player); //helper function, self explanatory
	bool isShielded(Screen& board, Point bombPos, Point targetPos) const; //helper function to check if an obstacle is shielding the target from the bomb explosion
};


