#pragma once
#include "Game.h"
#include "Steps.h"
#include "Results.h"
#include "Event.h"

class AutoGame :public Game
{
	bool isSilent;
	bool unmatching_result_found = false;
	unsigned int random_seed;
	Steps steps;
	Results results;
	std::string stepsFileName;
	std::string resultsFileName;
	Event lastEvent;
	
	void handleGameOver(bool& exitGame) override;
	void getFileNames();
	
public:
	AutoGame(bool isSilent);
	bool getInput(char& c, size_t iteration) override;
	void showMenu(bool& started) override;
	unsigned int getSeed() const { return random_seed; }
	void wait(int ms)override;
	void drawMap()override;
	void drawPlayer() override;
	void onGameEvent(const Event& e) override;
	void run();
	void handlePause(bool& exitGame) override;
};

