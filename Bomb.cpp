#include "Bomb.h"
#include "Screen.h"
#include "Utils.h"
void Bomb::explode(Screen& map) {
	map.showMessage("BOOM!");
	int bx = position.getX();
	int by = position.getY();
	for (int dy = -1; dy <= 1; dy++) {//destroys adjacent walls
		for (int dx = -1; dx <= 1; dx++) {
			if (dx == 0 && dy == 0) continue;
			point wallPos(bx + dx, by + dy);
			if (map.isWall(wallPos)) {
				map.setChar(wallPos, ' ');
			}
		}
	}
	for (int dy = -3; dy <= 3; dy++) {//3 tiles range to destroy objects
		for (int dx = -3; dx <= 3; dx++) {
			destroyCell(map, bx + dx, by + dy);
		}//TODO: handle player damage separately
	}
}

void Bomb::destroyCell(Screen& map, int x, int y) {
	if (x < 0 || x >= BOARD_DIMENSION::MAX_X || y < 0 || y >= BOARD_DIMENSION::MAX_Y) return;

	point target(x, y);
	if (map.isWall(target)) return;

	char c = map.getCharAt(target);
	if (c != ' ' && c != objSigns::PLAYER1 && c != objSigns::PLAYER2) {
		map.setChar(target, ' '); 
	}
	//TODO: player damage handling
}