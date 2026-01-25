#include "SaveGame.h"
#include <chrono>
#include <conio.h>

SaveGame::SaveGame() :Game()
{    
    setFileNames();
    if (stepsFileName == "") { stepsFileName = STEPS_FILE_NAME; }
    if (resultsFileName == "") { resultsFileName = RESULTS_FILE_NAME; }
    unsigned int generatedSeed = board.getSeed();
    steps.setRandomSeed(generatedSeed);
	board.setSeed(generatedSeed);
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
        steps.addStep(iteration, c);
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
    board.setSeed(steps.getRandomSeed());
	Game::performRestart();
}

void SaveGame::PerformGoToMenu(bool& exitGame)
{
    steps.clear();
    results.clear();
    board.setSeed(steps.getRandomSeed());
    Game::PerformGoToMenu(exitGame);
}

void SaveGame::changeRoom(int room)
{
    if (room == 1 && board.getCurrentRoom() == roomIndex::MENU) {
		board.setSeed(steps.getRandomSeed());
    }
	Game::changeRoom(room);
}

