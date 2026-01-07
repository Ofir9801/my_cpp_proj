#pragma once
#include "Utils.h"
#include <Windows.h>

class Point 
{
	int x, y;
	int diff_x = 0;
	int diff_y = 0;
	char ch;
public:
	Point(int _x = 0, int _y = 0, char _ch = ' '):x(_x), y(_y), ch(_ch){}//constractor with default values
	Point(int _x, int _y, objSigns _sign) :Point(_x, _y, static_cast<char>(_sign)) {}//constractor overload for objSigns

	int getX() const { return x; }
	int getY() const { return y; }
	void setX(int _x) { x = _x; }
	void setY(int _y) { y = _y; }
	void setChar(char _ch) { ch = _ch; }
	void setChar(objSigns _sign) { ch = static_cast<char>(_sign); }
	char getChar() const { return ch; }
	void draw(char c, Color color);
	void draw(objSigns sign, Color color) { draw(static_cast<char>(sign), color); }
	void draw(Color color) {draw(ch, color);}
	void move();
	void setDirection(Keyboard_bind dir, bool state = true, int speed = 1 );
	Keyboard_bind getDirectionEnum() const;
	bool operator== (const Point& otherPos) const;
	bool operator < (const Point& other) const; //for using Point as a key in map
	bool InBounds(int max_x = MAX_X, int max_y = MAX_Y, int min_x = 0, int min_y = 0) const;//making sure point is in bounds of the board
	void save(std::ofstream& file) const;
	void load(std::ifstream& file);
};
