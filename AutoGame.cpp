#include "AutoGame.h"
#include <filesystem>

AutoGame::AutoGame(bool isSilent) :isSilent(isSilent) {
    getFileNames();
    steps = Steps::loadSteps(stepsFileName);
    results = Results::loadResults(resultsFileName);
    unsigned int fileSeed = steps.getRandomSeed();
    board.setSeed(fileSeed);
}

bool AutoGame::getInput(char& c, size_t iteration)
{
    if (steps.isNextStepOnIteration(iteration)) {
        c = steps.popStep();
        return true;
    }
    return false;
    
}

void AutoGame::handleGameOver(bool& exitGame, size_t& gameCycle)
{
    reportResultError("Results file reached finish while game hadn't!", gameCycle);
    unmatching_result_found = true;
}

void AutoGame::getFileNames()
{
	std::vector<std::string> fileNames;
	getAllFilePaths(fileNames, ".steps");
	if (!fileNames.empty()) { stepsFileName = fileNames[0]; }
	fileNames.clear();
	getAllFilePaths(fileNames, ".result");
	if (!fileNames.empty()) { resultsFileName = fileNames[0]; }
}

bool AutoGame::isGameOver(size_t iteration) const
{
    return !results.isFinishedBy(iteration);
}

void AutoGame::showMenu(bool& started)
{
    changeRoom(/*get from file */ );
}

void AutoGame::drawMap() {
    if (!isSilent) Game::drawMap();
}
void AutoGame::drawPlayer() {
    if (!isSilent) Game::drawPlayer();
}

void AutoGame::onGameEvent(const Event& e){
    Event expected = results.popResult();
    size_t actualIteration = e.getIteration();
    EventType actualEventType = e.getEventType();
    char actualPlayer = e.getPlayer();

    if (expected.getIteration() != actualIteration || expected.getEventType() != actualEventType || expected.getPlayer() != actualPlayer) {
        reportResultError("Mismatch!", actualIteration);
    }
}

void AutoGame::wait(int ms) {
    if (!isSilent) Game::wait(ms);
}

