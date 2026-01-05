#include <iostream>
#include "Game.h"
#include "AutoGame.h"
#include <exception>

int main(int argc, char** argv) {
	bool isLoad = argc > 1 && std::string(argv[1]) == "-load";
	bool isSilent = isLoad && argc > 2 && std::string(argv[2]) == "-silent";

	try {
		if (isLoad) {
			AutoGame Game(isSilent);
			Game.run();
		}
		else {
			Game Game;
			Game.run();
		}
	}
	catch (const std::exception& e) {//catching any standard exceptions
		std::cout << "Exception caught: " << e.what() << std::endl;
	}
}