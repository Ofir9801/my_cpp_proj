#pragma once
#include "Game.h"
#include "Steps.h"
#include "Results.h"



class AutoGame :public Game
{
	bool isSilent;
	bool unmatching_result_found = false;
	unsigned int random_seed;
	Steps steps;
	Results results;
	std::string stepsFileName;
	std::string resultsFileName;
	
	void handleGameOver(bool& exitGame, size_t& gameCycle) override;
	void getFileNames();
	bool isGameOver(size_t iteration) const override;
public:
	AutoGame(bool isSilent);
	bool getInput(char& c, size_t iteration) override;
	void showMenu(bool& started) override;
	unsigned int getSeed() const { return random_seed; }
	void wait(int ms)override;
	void draw()override;
	void onGameEvent(const Event& e) override;
};

