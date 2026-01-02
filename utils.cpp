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
    coord.X = x;
    coord.Y = y;
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

string ReadRoomFromFile() {
	std::vector<std::string> roomFilePaths;
	getAllFilePaths(roomFilePaths, RoomsExtension, RoomsFolder); //change to const

	if (roomFilePaths.empty())
		return "Error: no files in directory that given";

	for (const auto& fullPath : roomFilePaths) {
		std::ifstream inFile(fullPath);
		if (!inFile.is_open()) {
			return "Error: Could not open file [" + fullPath + " ]";
		}
		roomIndex room;
		try {
			room = getRoomNumber(fullPath);
		}
		catch (const std::exception& e) {
			return std::string(e.what());
		}

		roomLegendRows[static_cast<int>(room)] = -1;
		int legendLocation = -1;
		string templine;
		bool gameRoom = isGameRoom(room);

		for (int i = 0; i < MAX_Y; i++) {
			if (!std::getline(inFile, templine)) {
				templine = "";
			}

			if (gameRoom) {
				size_t lPos = templine.find('L');
				if (lPos != string::npos) {
					if (lPos != 0)  return "Error in [" + fullPath + "] line " + std::to_string(i + 1) + ": Legend marker 'L' must be the start of the line";
					if (templine.length() > MAX_X) return "Error in[" + fullPath + "] line " + std::to_string(i + 1) + ": Legend line is too long(max " + std::to_string(MAX_X) + " chars)";

					legendLocation = i;
					roomLegendRows[static_cast<int>(room)] = i;
					i += LEGEND_SIZE - 1;
					continue;
				}
			}

			templine.resize(MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
			WriteLineToRoom(room, i, templine);
		}

		inFile.close();

		if (gameRoom) {
			if (legendLocation != -1) {
				try {
					ReadLegendFromFile(room, legendLocation);
				}
				catch (const std::runtime_error& e) {
					return "Error reading legend for [" + LegendPathWay + "]: " + string(e.what());
				}
			}
			else
				return "Error: L (represent for legend) is missing in [" + fullPath + "]";
		}
	}
	return "";
}

void ReadLegendFromFile(roomIndex room, int yOffset) {
	std::ifstream inFile(LegendPathWay);
	if (!inFile.is_open())
		throw std::runtime_error("Something wrong with the file Legend.txt");

	string templine;
	int linesRead = 0;
	while (linesRead < LEGEND_SIZE && std::getline(inFile,templine)){
		templine.resize(MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		int currentLine = yOffset + linesRead;
		WriteLineToRoom(room, currentLine, templine);
		linesRead++;
	}
	if (inFile >> templine) //Check if the legend file contains more lines than the defined limi
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
			switch (roomNum) {
				case 1:return roomIndex::ROOM1;
				case 2:return roomIndex::ROOM2;
				case 3:return roomIndex::ROOM3;
			}
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

void WriteLineToRoom(roomIndex room, int lineIndex, const std::string& text) {
	if (lineIndex < 0 || lineIndex >= MAX_Y) return;

	switch (room) {
	case roomIndex::MENU: Menu[lineIndex] = text; break;
	case roomIndex::INSTRUCTIONS: Instructions[lineIndex] = text; break;
	case roomIndex::ROOM1: Room1[lineIndex] = text; break;
	case roomIndex::ROOM2: Room2[lineIndex] = text; break;
	case roomIndex::ROOM3: Room3[lineIndex] = text; break;
	case roomIndex::VAULT: Vault[lineIndex] = text; break;
	case roomIndex::VICTORY: EndingScreen[lineIndex] = text; break;
	}
}