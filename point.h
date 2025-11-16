#pragma once
class point //classic point class for 2D coordinates
{
	int x, y;// private fields by default
	int diff_x = 0; //direction x
	int diff_y = 0; //direction y
	char ch; //character to draw
public:
	point(int _x = 0, int _y = 0, _ch = ' ') { x = _x; y = _y; ch = _ch; }//constractor with default values
	//some general functions, may remove those later
	int getX() const { return x; }
	int getY() const { return y; }
	void setX(int _x) { x = _x; }
	void setY(int _y) { y = _y; }
	void setChar(char _ch) { ch = _ch; }
	
	void draw() {
		draw(ch);
	}
	void draw(char c) {
		gotoxy(x, y);
		std::cout << c;
	}
	void move();
	void setDirection(Direction dir);

};

