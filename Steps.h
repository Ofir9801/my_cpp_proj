#pragma once
#include <list>
#include <string>

class Steps {
	long randomSeed = 0;
	bool colorMode = false;
	std::list<std::pair<size_t, char>> steps; // pair: iteration, step change to list of events
	static std::string readStepsFromFile(const std::string& filename, Steps& outSteps);
public:
	static Steps loadSteps(const std::string& filename);
	void saveSteps(const std::string& filename) const;
	long getRandomSeed() const {return randomSeed;}
	void setRandomSeed(long seed) {randomSeed = seed;}
	void addStep(size_t iteration, char step) {	steps.push_back({ iteration, step });}
	bool isColorMode()const { return colorMode; }
	void setColorMode(bool mode) { colorMode = mode; }
	bool isNextStepOnIteration(size_t iteration) const {return !steps.empty() && steps.front().first == iteration;}
	char popStep();
	bool isEmpty() { return steps.empty(); }
};

