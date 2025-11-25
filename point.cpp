#include "point.h"
#include "Screen.h"
#include "Keys.h"

void point::move() {
	x = (x + diff_x);// + Screen::MAX_X) % Screen::MAX_X;//OFIR: is this okay with the wall logic?
	y = (y + diff_y);// + Screen::MAX_Y) % Screen::MAX_Y;
}

void point::setDirection(Keys dir, int speed) { //SPEED MULTIPLY*****
	switch (dir) {
	case Keys::UP:
		diff_x = 0;
		diff_y = -1 * speed;
		break;
	case Keys::RIGHT:
		diff_x = 1 * speed;
		diff_y = 0;
		break;
	case Keys::DOWN:
		diff_x = 0;
		diff_y = 1 * speed;
		break;
	case Keys::LEFT:
		diff_x = -1 * speed;
		diff_y = 0;
		break;
	case Keys::STAY:
		diff_x = 0;
		diff_y = 0;
		break;
	}
}
