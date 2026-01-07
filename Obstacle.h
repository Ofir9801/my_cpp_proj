#pragma once
#include "Point.h"
#include <vector>
class Screen;//forward declaration to avoid circular dependency
class Obstacle
{
	Point position;
	Screen* board;
public:
	Obstacle(int x, int y, Screen* theMap): position(x, y, '*'), board(theMap) {}
	Obstacle():position(),board(nullptr){}
	bool push(int force, Keyboard_bind dir);
	Point getPosition() const { return position; }
	void save(std::ofstream& file) const;
	void load(std::ifstream& file);
	void setScreen(Screen* newBoard) { board = newBoard; }
private:
	void collectGroup(Point p, std::vector<Obstacle*>& group);// recursive function to gather connected obstacles
	void setPosition(const Point& p) { position = p; }
};

