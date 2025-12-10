#include <iostream>
#include "Game.h"
#include <conio.h>
#include "Utils.h"
#include "Player.h"
#include <windows.h>

using std::cout;
int main() {
	try {
		Screen board;
		Game Game;
		Game.run();
	}
	catch (const std::runtime_error& e) {
		std::cout << "Exception caught: " << e.what() << std::endl;
	}
}