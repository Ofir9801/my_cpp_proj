#include "SaveGame.h"
#include <chrono>
#include <conio.h>

SaveGame::SaveGame() :Game()
{    
    setFileNames();
    if (stepsFileName == "") { stepsFileName = "adv - world.steps "; }
    if (resultsFileName == "") { resultsFileName = "adv - world.result "; }
    unsigned int generatedSeed = board.getSeed();
    steps.setRandomSeed(generatedSeed);
}

SaveGame::~SaveGame()
{
    steps.setColorMode(board.IsColor());
    steps.saveSteps(stepsFileName);
    results.saveResults(resultsFileName);
}

void SaveGame::setFileNames()
{
    std::vector<std::string> fileNames;
    getAllFilePaths(fileNames, ".steps");
    if (!fileNames.empty()) { stepsFileName = fileNames[0]; }
    fileNames.clear();
    getAllFilePaths(fileNames, ".result");
    if (!fileNames.empty()) { resultsFileName = fileNames[0]; }   
}

bool SaveGame::getInput(char& c, size_t iteration)
{
    if (_kbhit()) {
        c = static_cast<char>(_getch());
        if (isImportantKey(c)) {
            steps.addStep(iteration, c);
        }
        return true;
    }
    return false;
}

void SaveGame::onGameEvent(const Event& e) {
    results.addResult(e);
}

void SaveGame::performRestart()
{
    steps.clear();
    results.clear();
	Game::performRestart();
}

void SaveGame::PerformGoToMenu(bool& exitGame)
{
    steps.clear();
    results.clear();
    Game::PerformGoToMenu(exitGame);
}

bool SaveGame::isImportantKey(char c) {
	char check = static_cast<char>(std::tolower(c));
    if (check == ESC || check == ENTER || check == BACKSPACE) { return true; }
    if (keys1.find(check) != string::npos) { return true; }
	if (keys2.find(check) != string::npos) { return true; }
    if (check >= '0' && check <= '9') { return true; }
    return false;
}

