#include "point.h"
#include "screen.h"
#include "keys.h"

void point::move() {
	x = (x + diff_x + screen::MAX_X) % screen::MAX_X;
	y = (y + diff_y + screen::MAX_Y) % screen::MAX_Y;
}

void point::setDirection(keys dir) {
	switch (dir) {
	case keys::UP:
		diff_x = 0;
		diff_y = -1;
		break;
	case keys::RIGHT:
		diff_x = 1;
		diff_y = 0;
		break;
	case keys::DOWN:
		diff_x = 0;
		diff_y = 1;
		break;
	case keys::LEFT:
		diff_x = -1;
		diff_y = 0;
		break;
	case keys::STAY:
		diff_x = 0;
		diff_y = 0;
		break;
	}
}
