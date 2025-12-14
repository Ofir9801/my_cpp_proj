#pragma once
#include "Point.h"
class Screen;//forward declaration to avoid circular dependency

class Door
{
	Point position;
	char id = 0;
	bool isOpen;
	Screen* board;
	bool connection = false; //indicates if the door is linked to a switch, default is false = no connection
public:
	Door(int x, int y, char _id, Screen* _board, bool connection = false) :
		position(Point(x, y, _id)),id(_id - '0'), isOpen(false), board(_board), connection(connection) {}

	void open();
	bool isAt(const Point& p) const {return position == p;}
	int getId() const { return id; }
	bool getIsOpen() const { return isOpen; }
	bool getConnection() const { return connection; }
	void setConnection(bool conn) { connection = conn; }
};