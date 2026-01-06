#include "AutoGame.h"
#include <filesystem>

AutoGame::AutoGame(bool isSilent) :isSilent(isSilent) {
    getFileNames();
    steps = Steps::loadSteps(stepsFileName);
    results = Results::loadResults(resultsFileName);
    unsigned int fileSeed = steps.getRandomSeed();
    board.setSeed(fileSeed);
    board.setSilentMode(isSilent);

    if (steps.isColorMode()) {
        SetColorfullGame();
    }
}

bool AutoGame::getInput(char& c, size_t iteration)
{
    if (steps.isNextStepOnIteration(iteration)) {
        c = steps.popStep();
        return true;
    }
    if (steps.isEmpty()) { //send char = ESC to force the game to pause
        c = ESC;
        return true;
    }
    return false;
    
}

void AutoGame::handleGameOver(bool& exitGame)
{
    onGameEvent(Event(gameCycle, EventType::GAME_OVER, ' ', "GameEnded. Score: " + std::to_string(board.getScore())));
    exitGame = false;
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

void AutoGame::showMenu(bool& started)
{
    started = true;
    changeRoom(roomIndex::ROOM1);
}

void AutoGame::drawMap() {
    if (!isSilent) Game::drawMap();
}
void AutoGame::drawPlayer() {
    if (!isSilent) Game::drawPlayer();
}

void AutoGame::onGameEvent(const Event& e){
    if (results.isEmpty()) {
        reportResultError("Mismatch!", e.getIteration());
        return;
    }
    Event expected = results.popResult();

    bool mismatch = (expected.getIteration() != e.getIteration()) ||
                    (expected.getEventType() != e.getEventType()) ||
                    (expected.getPlayer() != e.getPlayer());

    if ( mismatch) {
        std::string msg = "Mismatch! Expected: ";
      /*      + std::to_string(expected.getEventType()) +
            " at " + std::to_string(expected.getIteration()) +
            " | Got: " + std::to_string(e.getEventType()) +
            " at " + std::to_string(e.getIteration());*/
        reportResultError(msg, e.getIteration());
    }
}

void AutoGame::wait(int ms) {
    if (!isSilent) Game::wait(ms / 2);
}

void AutoGame::run() {
    if(isSilent){ std::cout << "Test Processing..." << std::flush; }
    Game::run(); // Run the normal game loop
    cls();
    
    std::cout << "\r" << std::flush;
    if (!results.isEmpty()) {
        reportResultError("Test Failed: Game ended but expected results still remain!", gameCycle);
    }
    else if (!unmatching_result_found) {
        // Only print success if no other errors were found
        std::cout << "Test Passed: Game replay matched perfectly." << std::endl;
    }
}

void AutoGame::handlePause(bool& exitGame)
{
    exitGame = false;
}
