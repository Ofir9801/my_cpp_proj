#pragma once
class point //classic point class for 2D coordinates
{
	int x, y;// private fields by default
public:
	point(int _x = 0, int _y = 0) { x = _x; y = _y; }//constractor with default values
	//some general functions, may remove those later
	int getX() { return x; }
	int getY() { return y; }
	void setX(int _x) { x = _x; }
	void setY(int _y) { y = _y; }

};

