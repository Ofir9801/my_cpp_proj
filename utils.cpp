#include <iostream>
#include <windows.h> // Required for Windows API console functions
#include <string>
#include <fstream>
#include <filesystem>
#include "Utils.h"

using std::string;

void gotoxy(int x, int y) {
    std::cout.flush();
	COORD coord;
    coord.X = static_cast<short>(x);
    coord.Y = static_cast<short>(y);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor(){
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE; // Set to TRUE to make it visible
    SetConsoleCursorInfo(hStdOut, &curInfo);
}

void cls() {
    system("cls");
}

void SetTextColor(Color color){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(color));
}

Color getColorForChar(char c){
	if (c == objSigns::PLAYER1 || c == objSigns::PLAYER2)
		return Color::CYAN;
	else if (c == objSigns::KEY)
		return Color::YELLOW;
	else if (c == objSigns::OBSTACLE)
		return Color::BROWN;
	else if (c == objSigns::SPRING)
		return Color::MAGENTA;
	else if (c == objSigns::SWITCH_ON)
		return Color::GREEN;
	else if (c == objSigns::SWITCH_OFF)
		return Color::RED;
	else if (c == objSigns::BOMB)
		return Color::LIGHTRED;
	else if (c == objSigns::RIDDLE)
		return Color::LIGHTGREEN;
	else if (c == objSigns::TORCH)
		return Color::YELLOW;
	else if (isdigit((unsigned char)c))
		return Color::DARKGREY;
	else
		return Color::WHITE;
}

Color getColorForChar(objSigns sign){
	return getColorForChar(static_cast<char>(sign));
}

RoomData ReadRoomFromFile(const string& fileName,bool exceptLegend) {
	RoomData result;
	std::ifstream inFile(fileName);
	
	// Read legend first
	std::vector<string> legendLines;
	if (exceptLegend) {
		legendLines = ReadLegendFromFile();
		if (legendLines.size() == 1 && legendLines[0].find("Error:") == 0) {
			result.errorMsg = legendLines[0];
			return result; // propagate the error
		}
	}

	if (!inFile.is_open()) {
		result.errorMsg = "Error: Could not open file [" + fileName + " ]";
		return result;
	}

	string templine;
	int currentLineCount = 0;

	while(std::getline(inFile,templine) && currentLineCount < MAX_Y) {
		if (!templine.empty() && templine.back() == '\r') {
			templine.pop_back();
		}
		if (exceptLegend) {
			if (!templine.empty() && templine[0] == 'D') {
				result.isDark = true;
				continue; //skip this line
			}
			if (!templine.empty() && templine[0] == 'E') {
				result.isExtraInventory = true;
				continue;
			}

			size_t lPos = templine.find('L');
			if (lPos != string::npos) { //legend found
				if (lPos != 0) { //legend marker not at start of line
					result.errorMsg = "Error: Legend marker 'L' must be at the start of the line in [" + fileName + "]";
					return result;
				}
				if (result.legendLoc != -1) { //legend already found before
					result.errorMsg = "Error: Multiple legend markers 'L' found in [" + fileName + "]";
					return result;
				}
				result.legendLoc = currentLineCount;

				for (const string& legendLine : legendLines) { //insert legend lines
					if (currentLineCount < MAX_Y) {
						result.layout.push_back(legendLine);
						currentLineCount++;
					}
				}
				continue;
			}
		}
		templine.resize(MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		result.layout.push_back(templine);
		currentLineCount++;
	}
	
	inFile.close();
	//if there is no legend in the file but it is required
	if (exceptLegend && result.legendLoc == -1) {
		result.errorMsg = "Error: Legend marker 'L' not found in [" + fileName + "]";
		return result;
	}
	if (result.layout.size() < MAX_Y) {
		result.errorMsg = "Error: Room file [" + fileName + "] has fewer than " + std::to_string(MAX_Y) + " lines.";
	}
	return result;
}

vector<string> ReadLegendFromFile(){ //fix it
	vector<string> legendLines;
	std::ifstream inFile(LegendPathWay);

	if (!inFile.is_open()) {
		legendLines.push_back("Error: Could not open file [" + LegendPathWay + " ]");
		return legendLines;
	}

	string templine;
	int lineCount = 0;
	while(std::getline(inFile,templine) && lineCount < LEGEND_SIZE) {
		if (!templine.empty() && templine.back() == '\r') {
			templine.pop_back();
		}

		templine.resize(MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		legendLines.push_back(templine);
		lineCount++;
	}
	inFile.close();

	// If the legend file has fewer than LEGEND_SIZE lines, fill the rest with empty lines
	while (legendLines.size() < LEGEND_SIZE){
		legendLines.push_back(string(MAX_X, ' '));
	}
	return legendLines;
}

void getAllFilePaths(std::vector<std::string>& vec_to_fill, std::string extension, std::string subFolder) {
	namespace fs = std::filesystem;
	fs::path fullPath = fs::current_path() / subFolder;
	if (fs::exists(fullPath)) {
		for (const auto& entry : fs::directory_iterator(fullPath)) {
			auto filename = entry.path().filename();
			if (filename.extension() == extension) {
				fs::path relativePath = fs::path(subFolder) / filename;
				vec_to_fill.push_back(relativePath.string());
			}
		}
	}
}

int getRoomNumber(std::string fileName) {
	std::string filename_prefix = std::filesystem::path(fileName).stem().string();

	if (filename_prefix == MenuPrefix)
		return static_cast<int>(roomIndex::MENU);
	if (filename_prefix == InstructionsPrefix)
		return static_cast<int>(roomIndex::INSTRUCTIONS);
	if (filename_prefix == VictoryPrefix)
		return static_cast<int>(roomIndex::VICTORY);
	if (filename_prefix == VaultPrefix)
		return static_cast<int>(roomIndex::VAULT);

	std::string lastTwoChars = "";

	if (filename_prefix.length() >= 2) {
		lastTwoChars = filename_prefix.substr(filename_prefix.length() - 2);
		if (std::isdigit(lastTwoChars[0]) && std::isdigit(lastTwoChars[1])) {
			int roomNum = std::stoi(lastTwoChars);
			return (roomNum);
		}
	}

	throw std::runtime_error("Error: "+ fileName+" isn't written by the guidelines");
}

//int getRoomNumberForState(std::string fileName)
//{
//	std::string filename_prefix = std::filesystem::path(fileName).stem().string();
//	std::string lastTwoChars = "";
//	int roomNum = -1;
//	if (filename_prefix.length() > 2) {
//		lastTwoChars = filename_prefix.substr(filename_prefix.length() - 2);
//		if (std::isdigit(lastTwoChars[0])) {
//			roomNum = (lastTwoChars[0] - '0') * 10 + (lastTwoChars[1] - '0');
//		}
//	}
//	if (roomNum > 0)
//		return roomNum;
//	return -1;
//}

