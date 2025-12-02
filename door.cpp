#pragma once
#include "Door.h"
#include "Screen.h"	


void Door::open() {
	if (!isOpen) {
		isOpen = true;
		//map.setChar(position, '{');//symbol of an open door
		map->showMessage("Door opened!");
	}
}
void Door::close() {
	if (isOpen) {
		isOpen = false;
		//map.setChar(position, char(id));//symbol of a closed door
		map->showMessage("Door closed!");
	}
}