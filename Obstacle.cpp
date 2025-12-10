#pragma once
#include "Obstacle.h"
#include "Screen.h"

void Obstacle::push(int force, Keys dir) {
	if (force < weight) {
		return; // Not enough force to move the obstacle
	}
	Point originalPos = position;
	position.setDirection(dir);
	position.move();
	char next = map->getCharAt(position);
	if (map->isWall(position) || isdigit(next)) {
		position = originalPos; // Revert to original position if blocked by a door or a wall
		position.setDirection(Keys::STAY);
		return;
	}
		
	
	map->setChar(originalPos, ' ');
	map->setChar(position, position.getChar()); // Set new position
}