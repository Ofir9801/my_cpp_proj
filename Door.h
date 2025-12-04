#pragma once
#include "point.h"
class Screen;//forward declaration to avoid circular dependency

class Door
{
	point position;
	char id = 0;
	bool isOpen;
	Screen* map;
public:
	Door(int x, int y, char _id, Screen* _map) :
		position(point(x, y, _id)),id(_id - '0'), isOpen(false), map(_map) {
	}

	void open();
	void close();
	bool isAt(const point& p) const {return position.getX() == p.getX() && position.getY() == p.getY(); }
	int getId() const { return id; }
	bool getIsOpen() const { return isOpen; }
};