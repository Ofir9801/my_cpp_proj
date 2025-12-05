#pragma once
#include "Door.h"
#include "Screen.h"	

void Door::open() {
	if (!isOpen) {
		isOpen = true;
		map->showMessage("Door opened!");
	}
}
