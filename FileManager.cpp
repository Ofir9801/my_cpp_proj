#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream> //why
#include <algorithm>
#include <iomanip>

namespace fs = std::filesystem;
void FileManager::saveRoomState(const RoomState& state, const string& filename, bool isFirstFile, int currentRoom, int lives, int score, bool colorToggle)
{
	std::ofstream file(filename);
	if (!file.is_open()) {
		return;
	}

	if (isFirstFile) {
		file << currentRoom << "\n";
		file << lives << "\n";
		file << score << "\n";
	}
	file << (colorToggle ? 1 : 0) << "\n";
	file << state.visited << "\n";

	saveVector(file, state.layout);
	saveVector(file, state.springs);
	saveVector(file, state.switches);
	saveVector(file, state.activeBombs);
	saveMap(file, state.obstacles);
	saveMap(file, state.riddles);
	saveMap(file, state.keys);
	saveMap(file, state.doors);

	file.close();
}

string FileManager::loadRoomState(int key, const string& filename, RoomState& state, int& currentRoom, int& lives, int& score, bool& colorToggle, Screen* screenContext)
{
	std::ifstream file(filename);
	if (!file.is_open()) return "Error: Could not open file [" + filename + " ]";

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//RoomState& state = savedRooms[key];
		if (key == 1) {
			file >> currentRoom;
			file >> lives;
			file >> score;

		}
		int flagColor;
		file >> flagColor;
		colorToggle = (flagColor == 1);
		file >> state.visited;
		file.ignore(); //ignore buffer

		loadVector(file, state.layout);
		loadVector(file, state.springs);
		loadVector(file, state.switches);
		loadVector(file, state.activeBombs);
		loadMapDataStructure(file, state.obstacles);
		loadMapDataStructure(file, state.riddles);
		loadMapDataStructure(file, state.keys);
		loadMapDataStructure(file, state.doors);

		if (screenContext) {
			for (auto& obs : state.obstacles) {
				obs.second.setScreen(screenContext);
			}
		}
	}
	catch (const std::exception& e) {
		return "Error reading file [" + filename + "]: " + e.what();
	}
	file.close();
	return "";
}

void FileManager::setFileName(string& file, const int key, const string& folderPath)
{
	string roomNumber = (key < 10) ? "0" + std::to_string(key) : std::to_string(key);
	file = folderPath + "/" + "SavedScreen" + roomNumber + STATE_EXTENSION;
}

string FileManager::getCurrentTimeStamp()
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);

	std::tm timeInfo;
	localtime_s(&timeInfo, &now_c);

	std::stringstream ss;
	ss << std::put_time(&timeInfo, "Save_%d-%m-%Y_%H-%M-%S");
	return ss.str();;
}

string FileManager::formatTime(fs::file_time_type ftime)
{
	auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
	std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
	std::tm tm;
	localtime_s(&tm, &tt);
	std::stringstream ss;
	ss << std::put_time(&tm, "%d/%m/%y %H:%M");
	return ss.str();
}

string FileManager::saveGame(const map<int, RoomState>& rooms, int lives, int score, int currentRoom, bool colorToggle, const string& prevPath)
{
	const string rootSaves = STATE_FOLDER;

	//create folder if not exists already
	if (!fs::exists(rootSaves)) {
		fs::create_directory(rootSaves);
	}

	//if there is a current save directory, delete it first so we dont have multiple saves from the same session
	if (!prevPath.empty() && fs::exists(prevPath)) {
		fs::remove_all(prevPath); // Deletes folder and all files inside
	}
	
	 //check if there is more than 10 files and if so erase the oldest one
	vector<fs::path> existingSaves;
	for (const auto& entry : fs::directory_iterator(rootSaves)) {
		if (entry.is_directory()) {
			existingSaves.push_back(entry.path());
		}
	}

	if (existingSaves.size() >= MAX_SAVED_SLOTS) {
		auto oldest = existingSaves.end();
		fs::file_time_type oldestTime = (fs::file_time_type::max)();

		for (auto it = existingSaves.begin(); it != existingSaves.end(); ++it) {
			auto lastWrite = fs::last_write_time(*it);
			if (lastWrite < oldestTime) {
				oldestTime = lastWrite;
				oldest = it;
			}
		}

		if (oldest != existingSaves.end()) {
			fs::remove_all(*oldest);
		}
	}
	
	string currentSaveName = getCurrentTimeStamp();
	string fullSavePath = rootSaves + "/" + currentSaveName;
	fs::create_directory(fullSavePath);

	string file = "";
	for (const auto& roomPair : rooms) {
		setFileName(file, roomPair.first, fullSavePath);
		bool isFirst = (roomPair.first == 1);
		saveRoomState(roomPair.second, file, isFirst,currentRoom,lives,score,colorToggle); 
	}
	return fullSavePath;
}


string FileManager::loadGame(map<int, RoomState>& rooms, int& lives, int& score, const string& folderPath, int& currentRoom, bool& colorToggle, Screen* screenContext)
{
	if (folderPath.empty()) return "Error: Empty folder path";

	rooms.clear();
	vector<string> filesInFolder;
	getAllFilePaths(filesInFolder, STATE_EXTENSION, folderPath);

	string errorMsg = "";

	for (auto& file : filesInFolder) {
		int key = getRoomNumber(file);
		if (key == -1) {
			return "Error: file isnt by designated format - [" + file + "]";
		}

		RoomState& state = rooms[key]; //creates new entry if not exists		
		errorMsg = loadRoomState(key, file, state, currentRoom, lives, score, colorToggle, screenContext);

		if (!errorMsg.empty())
			return errorMsg;
	}
	return "";
}


void FileManager::cleanSavedGames()
{
	string rootFolder = STATE_FOLDER;
	if (fs::exists(rootFolder)) {
		fs::remove_all(rootFolder); // Deletes folder and all files inside
	}
}

vector<FileManager::SaveFileEntry> FileManager::getSavedGamesList()
{
	vector<SaveFileEntry> saves;
	string rootFolder = STATE_FOLDER;

	if (!fs::exists(rootFolder)) {
		return saves;
	}
	for (const auto& entry : fs::directory_iterator(rootFolder)) {
		if (entry.is_directory()) {
			SaveFileEntry saveEntry;
			saveEntry.folderName = entry.path().string();
			saveEntry.displayName = entry.path().filename().string();
			saveEntry.rawTime = entry.last_write_time();
			saveEntry.timestampStr = formatTime(saveEntry.rawTime);
			saves.push_back(saveEntry);
		}
	}

	// Sort by last modified time, newest first
	std::sort(saves.begin(), saves.end(), [](const SaveFileEntry& a, const SaveFileEntry& b) {
		return a.rawTime > b.rawTime; // Newest first
	});
	return saves;
}
