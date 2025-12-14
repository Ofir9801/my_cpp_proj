#pragma once
#include "Obstacle.h"
#include "Screen.h"

void Obstacle::push(int force, Keyboard_bind dir) {
	if (force < weight) {
		return; // Not enough force to move the obstacle
	}
	Point originalPos = position;
	position.setDirection(dir);
	position.move();
	char next = board->getCharAt(position);
	if (board->isWall(position) || isdigit(next)) {
		position = originalPos; // Revert to original position if blocked by a door or a wall
		position.setDirection(Keyboard_bind::STAY);
		return;
	}
		
	
	board->setChar(originalPos, ' ');
	board->setChar(position, position.getChar()); // Set new position
}