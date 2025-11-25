#include <iostream>
#include "Game.h"
#include <conio.h>
#include "utils.h"
#include "player.h"
#include <windows.h>
#include "objSigns.h"
#include "Rooms.h"

using std::cout;
int main() {
	const char keys1[NUM_KEYS + 1] = "wdsaeq";
	const char keys2[NUM_KEYS + 1] = "ilkjou";

	screen board;
	Game game;
	game.run();

}