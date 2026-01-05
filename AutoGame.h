#pragma once
#include "Game.h"
#include "Steps.h"
#include "Results.h"



class AutoGame :public Game
{
	bool isSilent;
	long random_seed;
	Steps steps;
	Results results;
	std::string stepsFilename;
	std::string resultsFilename;
	void getFileNames();
public:
	AutoGame(bool isSilent) :isSilent(isSilent) {
		//which mode we are
		getFileNames();
		steps = Steps::loadSteps(stepsFilename);
		random_seed = steps.getRandomSeed();
		results = Results::loadResults(resultsFilename);

	}
	bool getInput(char& c) override;
	void showMenu(bool& started) override;

};

