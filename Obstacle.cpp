#include "Obstacle.h"

void Obstacle::push(int force, Keys dir) {
	if (force < weight) {
		return; // Not enough force to move the obstacle
	}
	point originalPos = position;
	position.setDirection(dir);
	position.move();
	char next = map.getCharAt(position);
	if (map.isWall(position) || isdigit(next)) {
		position = originalPos; // Revert to original position if blocked by a door or a wall
		position.setDirection(Keys::STAY);
		return;
	}
	// Update the map to reflect the new position of the obstacle
	map.setChar(originalPos, ' '); // Clear old position
	map.setChar(position, position.getChar()); // Set new position
}