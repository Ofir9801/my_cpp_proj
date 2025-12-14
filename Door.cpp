#pragma once
#include "Door.h"
#include "Screen.h"	

void Door::open() {
	if (!isOpen) {
		isOpen = true;
		board->showMessage("Door opened!");
	}
}
