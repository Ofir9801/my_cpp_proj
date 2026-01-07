#pragma once
#include "Point.h"

class Door
{
	Point position;
	char id = 0;
	bool isOpen;
	bool connection = false; //indicates if the door is linked to a switch, default is false = no connection
public:
	Door() : position(0, 0, ' '), id(0), isOpen(false), connection(false) {} //default constructor for empty door for map
	Door(int x, int y, char _id, bool connection = false) :
		position(Point(x, y, _id)),id(_id - '0'), isOpen(false), connection(connection) {}
	void open() { isOpen = true;};
	bool getIsOpen() const { return isOpen; }
	bool getConnection() const { return connection; }
	void setConnection(bool conn) { connection = conn; }
	void save(std::ofstream& file) const;
	void load(std::ifstream& file);
};