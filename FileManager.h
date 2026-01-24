#pragma once
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include "utils.h"
#include "roomState.h"

class Screen;

using std::string, std::map, std::vector;
class FileManager
{
	static void saveRoomState(const RoomState& state, const string& filename, bool isFirstFile, int currentRoom, int lives, int score, bool colorToggle);
	static string loadRoomState(int key, const string& filename, RoomState& state, int& currentRoom, int& lives, int& score, bool& colorToggle, Screen* screenContext);
	static void setFileName(string& file, const int key, const string& folderPath);
	static string getCurrentTimeStamp();
	static string formatTime(string fileName);

public:
    static string saveGame(const map<int, RoomState>& rooms, int lives, int score, int currentRoom, bool colorToggle, const string& prevPath);
    static string loadGame(map<int, RoomState>& rooms, int& lives, int& score, const string& fileName, int&currentRoom, bool& colorToggle, Screen* screenContext);
    static void cleanSavedGames();
	struct SaveFileEntry {
		string folderName;
		string displayName;
		string timestampStr;
		std::filesystem::file_time_type rawTime;
	};
	static vector<SaveFileEntry> getSavedGamesList();
};

