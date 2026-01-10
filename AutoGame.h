#pragma once
#include "Game.h"
#include "Steps.h"
#include "Results.h"
#include "Event.h"
#include <list>

class AutoGame :public Game
{
	bool isSilent;
	unsigned int random_seed;
	Steps steps;
	Results results;
	std::string stepsFileName;
	std::string resultsFileName;
	Event lastEvent;
	std::list<std::pair<Event,Event>> mismatchEvents; //use for showing the user all the mismatching between the saved game and the game that was running (if the user tamper with the results files)
	void handleGameOver(bool& exitGame) override;
	void getFileNames();
	void printList()const;
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

