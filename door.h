#pragma once
#include "point.h"
#include "Screen.h"
#include "objSigns.h"

class Door
{
	point position;
	int id = 0;
	bool isOpen;
	Screen& map;
public:
	Door(int x, int y, int _id, Screen& _map) :
		position(point(x, y, _id)), isOpen(false), map(_map) {
	}

	void open();
	void close();
	bool isAt(const point& p) const {return position.getX() == p.getX() && position.getY() == p.getY(); }
	int getId() const { return id; }
	bool getIsOpen() const { return isOpen; }
};