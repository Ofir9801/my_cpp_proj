#include "Point.h"
#include "Screen.h"

void Point::draw(char c, int color)
{
	gotoxy(x, y);
	SetTextColor(color);
	std::cout << c;
	SetTextColor((int)Color::WHITE); //reset to default color
}

void Point::move() {
	x = (x + diff_x);
	y = (y + diff_y);
}

void Point::setDirection(Keyboard_bind dir, bool state, int speed)
{
	if (state) {
		switch (dir) {
		case Keyboard_bind::UP:
			diff_x = 0;
			diff_y = -1 * speed;
			break;
		case Keyboard_bind::RIGHT:
			diff_x = 1 * speed;
			diff_y = 0;
			break;
		case Keyboard_bind::DOWN:
			diff_x = 0;
			diff_y = 1 * speed;
			break;
		case Keyboard_bind::LEFT:
			diff_x = -1 * speed;
			diff_y = 0;
			break;
		case Keyboard_bind::STAY:
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
Keyboard_bind Point::getDirectionEnum() const
{
	if (diff_y < 0) {
		return Keyboard_bind::UP;
	}
	else if (diff_x > 0) {
		return Keyboard_bind::RIGHT;
	}
	else if (diff_y > 0) {
		return Keyboard_bind::DOWN;
	}
	else if (diff_x < 0) {
		return Keyboard_bind::LEFT;
	}
		return Keyboard_bind::STAY;
}

bool Point::operator== (const Point& otherPos) const {
	if (x == otherPos.x && y == otherPos.y)
		return true;
	return false;
}

bool Point::operator<(const Point& other) const
{
	if (y != other.y) return y < other.y;
	return x < other.x;
}