#pragma once
#include "Game.h"
class SaveGame :public Game
{
	Steps steps;
	Results results;
	std::string stepsFileName;
	std::string resultsFileName;
	bool isImportantKey(char c);
	void setFileNames();
public:
	SaveGame();
	~SaveGame();
	bool getInput(char& c, size_t iteration = 0) override;
	void onGameEvent(const Event& e) override;
	bool isSaveLoadAllowed() const override { return false; }
	void performRestart() override;
	void PerformGoToMenu(bool& exitGame) override;
};

