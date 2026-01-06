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
		system("cls");
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
	}
}
std::string Steps::readStepsFromFile(const std::string& filename, Steps& outSteps){
	outSteps = Steps(); // Reset object
	std::ifstream steps_file(filename);

	if (!steps_file.is_open()) {
		return "Error: Could not open file [" + filename + "]";
	}

	steps_file >> outSteps.randomSeed;
	if (steps_file.fail()) return "Error: Invalid format (Seed missing)";

	int colorFlag = 0;
	steps_file >> colorFlag;
	if (steps_file.fail()) return "Error: Invalid format (Color Flag missing)";
	outSteps.colorMode = (colorFlag == 1);

	size_t size;
	steps_file >> size;
	if (steps_file.fail()) return "Error: Invalid format (Size missing)";

	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char step;
		steps_file >> iteration >> step;
		if (steps_file.fail()) break;
		outSteps.addStep(iteration, step);
	}
	steps_file.close();
	return "";
}


void Steps::saveSteps(const std::string& filename) const {
	std::ofstream steps_file(filename);
	steps_file << randomSeed << ' ' << (colorMode ? 1 : 0) << '\n';
	steps_file << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.first << ' ' << step.second;
	}
	steps_file.close();
}
char Steps::popStep() {
	char step = steps.front().second;
	steps.pop_front();
	return step;
}
