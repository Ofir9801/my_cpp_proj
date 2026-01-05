#include "SaveGame.h"
#include <chrono>
#include <conio.h>

SaveGame::SaveGame() :Game()
{    
    setFileNames();
    unsigned int generatedSeed = board.getSeed();
    steps.setRandomSeed(generatedSeed);
}

SaveGame::~SaveGame()
{
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
        c = _getch();
        if (ImportantkeyPressed(c)) {
            steps.addStep(iteration, c);
        }
        return true;
    }
    return false;
}

void SaveGame::onGameEvent(const Event& e) {
    results.addResult(e);
}

