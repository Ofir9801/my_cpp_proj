#include "point.h"
#include "Screen.h"

void point::draw(char c, int color)
{
	gotoxy(x, y);
	SetTextColor(color);
	std::cout << c;
	SetTextColor(WHITE); //reset to default color
}

void point::move() {
	x = (x + diff_x);
	y = (y + diff_y);
}

void point::setDirection(Keys dir, bool state, int speed)
{
	if (state) {
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
	else {
		diff_x = 0;
		diff_y = 0;
	}
}
Keys point::getDirectionEnum() const
{
	if (diff_y < 0) {
		return Keys::UP;
	}
	else if (diff_x > 0) {
		return Keys::RIGHT;
	}
	else if (diff_y > 0) {
		return Keys::DOWN;
	}
	else if (diff_x < 0) {
		return Keys::LEFT;
	}
		return Keys::STAY;
}