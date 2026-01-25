#include <fstream>
#include "Steps.h"
#include <conio.h>
#include <iostream>
#include "utils.h"

Steps Steps::loadSteps(const std::string& filename) {
	Steps steps;
	std::string errorMsg;

	while (true) {
		errorMsg = readStepsFromFile(filename, steps);

		if (errorMsg.empty()) {
			return steps; // Success
		}	
		std::cout << "########################################################" << std::endl;
		std::cout << "ERROR LOADING STEPS FILE" << std::endl;
		std::cout << "########################################################" << std::endl;
		std::cout << errorMsg << std::endl << std::endl;
		std::cout << "1. Fix the file externally." << std::endl;
		std::cout << "2. Press 'r' to RETRY." << std::endl;
		std::cout << "3. Press 'ESC' to EXIT Game." << std::endl;

		char c = static_cast<char>(_getch());
		if (c == ESC) {
			throw std::runtime_error("Game stopped by user due to missing Steps file.");
		}
		system("cls");
	}
}
std::string Steps::readStepsFromFile(const std::string& filename, Steps& outSteps){
	outSteps = Steps(); // Reset object
	std::ifstream steps_file(filename);

	if (!steps_file.is_open()) {
		return "Error: Could not open file [" + filename + "]";
	}

	steps_file >> outSteps.randomSeed;
	if (steps_file.fail()) return "Error: Invalid format - Seed missing";

	int colorFlag = 0;
	steps_file >> colorFlag;
	if (steps_file.fail()) return "Error: Invalid format - Color Flag missing";
	outSteps.colorMode = (colorFlag == 1);

	size_t expectedSize;
	steps_file >> expectedSize;
	if (steps_file.fail()) return "Error: Invalid format - Size missing";

	size_t actualCount = 0;
	while (!steps_file.eof()) {
		size_t iteration;
		int stepValue;
		if(!(steps_file >> iteration >>stepValue)) break;
		outSteps.addStep(iteration, static_cast<char>(stepValue));
		actualCount++;
	}
	if (expectedSize != actualCount) return "Error: Invalid format - the number of steps: "+ std::to_string(actualCount) + ", don't match the size: " + std::to_string(expectedSize);
	steps_file.close();
	return "";
}


void Steps::saveSteps(const std::string& filename) const {
	std::ofstream steps_file(filename);
	steps_file << randomSeed << ' ' << (colorMode ? 1 : 0) << '\n';
	steps_file << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.first << ' ' << static_cast<int>(step.second);
	}
	steps_file.close();
}
char Steps::popStep() {
	char step = steps.front().second;
	steps.pop_front();
	return step;
}
