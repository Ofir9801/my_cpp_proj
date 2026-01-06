#pragma once
#include <list>
#include <string>

class Steps {
	long randomSeed = 0;
	bool colorMode = false;
	std::list<std::pair<size_t, char>> steps; // pair: iteration, step change to list of events
public:
	static Steps loadSteps(const std::string& filename);
	void saveSteps(const std::string& filename) const;
	long getRandomSeed() const {
		return randomSeed;
	}
	void setRandomSeed(long seed) {
		randomSeed = seed;
	}
	void addStep(size_t iteration, char step) {
		steps.push_back({ iteration, step });
	}
	bool isColorMode()const { return colorMode; }
	void setColorMode(bool mode) { colorMode = mode; }
	bool isNextStepOnIteration(size_t iteration) const {
		return !steps.empty() && steps.front().first == iteration;
	}
	char popStep() {
		char step = steps.front().second;
		steps.pop_front();
		return step;
	}
	bool isEmpty() { return steps.empty(); }
};

