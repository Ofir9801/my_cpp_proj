#include "AutoGame.h"
#include <filesystem>

void AutoGame::getFileNames()
{
    std::vector<std::string> fileNames;
    getAllFilePaths(fileNames, ".steps");
    if (!fileNames.empty()) { stepsFilename = fileNames[0]; }
    fileNames.clear();
    getAllFilePaths(fileNames, ".result");
    if (!fileNames.empty()){ resultsFilename = fileNames[0]; }
}

bool AutoGame::getInput(char& c)
{
    if (moves.empty()) return false; // נגמרו המהלכים בקובץ

    c = moves.front();
    moves.pop();

    // אופציונלי: הוסף השהיה קטנה כדי שתוכל לראות את המשחק רץ בעיניים ולא בשבריר שנייה
    Sleep(50);

    return true;
    //getfrom file
}

void AutoGame::showMenu(bool& started)
{
    changeRoom(/*get from file */ );
}
