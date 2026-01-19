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

std::vector<string> ReadRoomFromFile(const string& fileName, int& legendLoc) {
	std::vector<string> roomFile;
	std::ifstream inFile(fileName);
	if (!inFile.is_open()) {
		roomFile.push_back("Error: Could not open file [" + fileName + " ]");
		return roomFile;
	}

	string templine;

	for (int i = 0; i < MAX_Y; i++) {
		if (!std::getline(inFile, templine)) {
			templine = "";
		}
		size_t lPos = templine.find('L');
		if (lPos != string::npos) { //find in line the letter L that represent the legend
			legendLoc = i;
			//validation legend here


			/*try {
				ReadLegendFromFile(roomFile,lPos, i);
				addLegend = true;
			}
			catch (const std::runtime_error& e) {
				roomFile.clear();
				roomFile.push_back("Error reading legend for [" + fileName + "]: " + string(e.what()));
				return roomFile;
			}
			i += roomFile.size() - prevSize;
			continue;
		}*/
		templine.resize(MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		roomFile.push_back(templine);			
	}
	inFile.close();

	/*if (gameRoom && !addLegend) {
		roomFile.clear();
		roomFile.push_back("Error: L (represent for legend) is missing in [" + fileName + "]");
		return roomFile;
	}*/
	return roomFile;
}
	
void ReadLegendFromFile(std::vector<string>& roomFile, size_t lPos, int currentLine) {
	std::ifstream inFile(LegendPathWay);
	if (lPos != 0) {
		throw "In line " + std::to_string(currentLine + 1) + ": Legend marker 'L' must be the start of the line";
	}
	if (!inFile.is_open())
		throw std::runtime_error("Something wrong with the file Legend.txt");

	string templine;
	int linesRead = 0;
	while (linesRead < LEGEND_SIZE && std::getline(inFile,templine)){
		if (templine.length() > MAX_X) {
			throw "In line " + std::to_string(linesRead + currentLine + 1) + ": Legend line is too long(max " + std::to_string(MAX_X) + " chars)";
		}
		templine.resize(MAX_X, ' ');//if the line is shorter then MAX_X, it add space bars to fill the missing places
		roomFile.push_back(templine);
		linesRead++;
	}
	if (inFile >> templine) //Check if the legend file contains more lines than the defined limit
		throw std::runtime_error("The legend size exceeds the maximum defined lines: "+std::to_string(LEGEND_SIZE)+" lines");
	inFile.close();
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

roomIndex getRoomNumber(std::string fileName) {
	std::string filename_prefix = std::filesystem::path(fileName).stem().string();
	std::string lastTwoChars = "";

	if (filename_prefix.length() > 2) {
		lastTwoChars = filename_prefix.substr(filename_prefix.length() - 2);
		if (std::isdigit(lastTwoChars[0])) {
			int roomNum = (lastTwoChars[0] - '0') * 10 + (lastTwoChars[1] - '0');
			return static_cast<roomIndex> (roomNum);
		}
	}

	if (filename_prefix == MenuPrefix)
		return roomIndex::MENU;
	if (filename_prefix == InstructionsPrefix)
		return roomIndex::INSTRUCTIONS;
	if (filename_prefix == VictoryPrefix)
		return roomIndex::VICTORY;
	if (filename_prefix == VaultPrefix)
		return roomIndex::VAULT;

	throw std::runtime_error("Error: "+ fileName+" isn't written by the guidelines");
}

int getRoomNumberForState(std::string fileName)
{
	std::string filename_prefix = std::filesystem::path(fileName).stem().string();
	std::string lastTwoChars = "";
	int roomNum = -1;
	if (filename_prefix.length() > 2) {
		lastTwoChars = filename_prefix.substr(filename_prefix.length() - 2);
		if (std::isdigit(lastTwoChars[0])) {
			roomNum = (lastTwoChars[0] - '0') * 10 + (lastTwoChars[1] - '0');
		}
	}
	if (roomNum > 0)
		return roomNum;
	return -1;
}

