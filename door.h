#pragma once
#include "point.h"
class Screen;//forward declaration to avoid circular dependency

class Door
{
	point position;
	char id = 0;
	bool isOpen;
	Screen* map;
	bool connection = false; //indicates if the door is linked to a switch, default is false = no connection
public:
	Door(int x, int y, char _id, Screen* _map, bool connection = false) :
		position(point(x, y, _id)),id(_id - '0'), isOpen(false), map(_map), connection(connection) {
	}

	void open();
	void close();
	bool isAt(const point& p) const {return position.getX() == p.getX() && position.getY() == p.getY(); }
	int getId() const { return id; }
	bool getIsOpen() const { return isOpen; }
	bool getConnection() const { return connection; }
	void setConnection(bool conn) { connection = conn; }
};