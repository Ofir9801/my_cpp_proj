#include <iostream>
#include "Game.h"
#include <conio.h>
#include "Utils.h"
#include "Player.h"
#include <exception>

int main() {
	try {
		Screen board;
		Game Game;
		Game.run();
	}
	catch (const std::exception& e) {//catching any standard exceptions
		std::cout << "Exception caught: " << e.what() << std::endl;
	}
}