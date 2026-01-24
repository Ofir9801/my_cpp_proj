#include "AutoGame.h"
#include <filesystem>

AutoGame::AutoGame(bool isSilent) :isSilent(isSilent) {
    getFileNames();
    steps = Steps::loadSteps(stepsFileName);
    results = Results::loadResults(resultsFileName);
    unsigned int fileSeed = steps.getRandomSeed();
    board.setSeed(fileSeed);
    board.setSilentMode(isSilent);
    mismatchEvents.clear();
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
    std::string winMsg = "";
    if (board.getCurrentRoom() == roomIndex::VICTORY) {
        winMsg = "The User won the game";
    }
    else {
        winMsg = "The User lost the game";
    }
    onGameEvent(Event(gameCycle, EventType::GAME_OVER, ' ', " Game Ended: " + winMsg +". The score is : " + std::to_string(board.getScore())));
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

void AutoGame::printList() const
{
    for (auto& e : mismatchEvents) {
        std::cout << "Expected - " << e.first.printEvent() << std::endl;
        std::cout << "Actual - " << e.second.printEvent() << "\n" << std::endl;
    }
    
}

void AutoGame::showMenu(bool& started)
{
    started = true;
    changeRoom(1);
}

void AutoGame::drawMap() {
    if (!isSilent) Game::drawMap();
}
void AutoGame::drawPlayer() {
    if (!isSilent) Game::drawPlayer();
}

void AutoGame::onGameEvent(const Event& e){
    if (results.isEmpty()) {
        //reportResultError("Mismatch!", e.getIteration());
        return;
    }
    if (results.size() == 1) {
        lastEvent = results.TopResult();
    }
    
    Event expected = results.popResult();

    bool mismatch = (expected.getIteration() != e.getIteration()) ||
                    (expected.getEventType() != e.getEventType()) ||
                    (expected.getPlayer() != e.getPlayer());

    if ( mismatch) {
        std::string msg = "Mismatch!";
        mismatchEvents.push_back({ expected,e });
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
    else if (mismatchEvents.empty()){
        // Only print success if no other errors were found
        std::cout << "Test Passed: Game replay matched perfectly." << std::endl;
        std::cout << lastEvent.getPayload() << std::endl;
    }
    else {
        std::cout << "Test Failed, there is few mismatches:" << std::endl;
        printList();
    }
}

void AutoGame::handlePause(bool& exitGame)
{
    exitGame = false;
}
