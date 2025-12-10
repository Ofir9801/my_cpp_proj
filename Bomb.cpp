#include "Bomb.h"
#include "Screen.h"
#include "Utils.h"
#include <cmath>

void Bomb::explode(Screen& map) {
	active = false;
	map.showMessage("BOOM! The bomb exploded.");
	map.setChar(position, ' ');
	int bx = position.getX();
	int by = position.getY();
	for (int dy = -3; dy <= 3; dy++) {
		for (int dx = -3; dx <= 3; dx++) {
			int distance = (std::abs(dx) > std::abs(dy)) ? std::abs(dx) : std::abs(dy);//calculating distance properly
			destroyCell(map, bx + dx, by + dy, distance);//helper function ith correct distance each time
		}
	}
}

void Bomb::destroyCell(Screen& map, int x, int y, int distance) {
	if (x < 0 || x >= BOARD_DIMENSION::MAX_X || y < 0 || y >= BOARD_DIMENSION::MAX_Y) return;

	Point target(x, y);
	if (map.isWall(target)){
		if (distance <= 1) 
			map.setChar(target, ' ');
		return;
	}
	/*if (map.isPlayerAt(target)) {
		map.resetPlayerAt(target);
		return;
	}*/

	char c = map.getCharAt(target);
	if (c != ' ') {
		map.setChar(target, ' ');
	}
	//TODO: player damage handling
}