#pragma once
#include "Utils.h"
#include <iostream>
#include <Windows.h>

class Point //classic Point class for 2D coordinates
{
	int x, y;// private fields by default
	int diff_x = 0; //direction x
	int diff_y = 0; //direction y
	char ch; //character to draw
public:
	Point(int _x = 0, int _y = 0, char _ch = ' ') { x = _x; y = _y; ch = _ch; }//constractor with default values
	//some general functions, may remove those later
	int getX() const { return x; }
	int getY() const { return y; }
	void setX(int _x) { x = _x; }
	void setY(int _y) { y = _y; }
	void setChar(char _ch) { ch = _ch; }
	char getChar() const { return ch; }
	void draw(char c, Color color);
	void draw(Color color) {draw(ch, color);}
	void move();
	void setDirection(Keyboard_bind dir, bool state = true, int speed = 1 );
	Keyboard_bind getDirectionEnum() const;
	bool operator== (const Point& otherPos) const;
	bool operator < (const Point& other) const;
	bool InBounds(int max_x = MAX_X, int max_y = MAX_Y, int min_x = 0, int min_y = 0) const;
};
