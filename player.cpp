#include "player.h"
#include "screen.h"
#include "keys.h"


void player::handleKeyPressed(char key_pressed) {
	size_t index = 0;
	for (char k : p_keys) {
		if (std::tolower(k) == std::tolower(key_pressed)) {
			position.setDirection((keys)index);
			return;
		}
		++index;
	}
}

void player::move() {
	position.draw(' ');
	point body0orig = position;
	position.move();
	if (map.isWall(position)) {
		position = body0orig;
	}
	position.draw();
}