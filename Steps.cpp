#include <fstream>

#include "Steps.h"

Steps Steps::loadSteps(const std::string& filename) {
	Steps steps;
	std::ifstream steps_file(filename);
	
	if (!steps_file.is_open()) return steps; // should alert the user. flag

	steps_file >> steps.randomSeed;
	int colorFlag = 0;
	steps_file >> colorFlag;
	steps.colorMode = (colorFlag == 1);
	size_t size;
	steps_file >> size;
	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char step;
		steps_file >> iteration >> step;
		steps.addStep(iteration, step);
	}
	steps_file.close();
	return steps;
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
