#include <iostream>
#include "Game.h"
#include "AutoGame.h"
#include "SaveGame.h"
#include <exception>

int main(int argc, char** argv) {
	try{
		if (argc > 1) {
			if (std::string(argv[1]) == "-load") {
				bool isSilent = argc > 2 && std::string(argv[2]) == "-silent";
				AutoGame Game(isSilent);
				Game.run();
			}
			else { //"argv[1]) == "-save"
				SaveGame Game;
				Game.run();
			}
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