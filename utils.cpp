#include <iostream>
#include <windows.h> // Required for Windows API console functions
#include <cstdlib>
#include <string>
#include <fstream>
#include "Utils.h"

using std::string;

void gotoxy(int x, int y) {
    std::cout.flush();
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE; // Set to TRUE to make it visible
    SetConsoleCursorInfo(hStdOut, &curInfo);
}

void cls() {
    system("cls");
}

void SetTextColor(Color color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
}

Color getColorForChar(char c)
{
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
Color getColorForChar(objSigns sign)
{
	return getColorForChar(static_cast<char>(sign));
}

string ReadRoomLayoutFromFile(string FileName, roomIndex room){
	std::ifstream inFile(FileName);
	if (!inFile.is_open()) {
		return "Error: Could not open file [" + FileName + " ]";
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
				if (lPos != 0) {
					return "Error in [" + FileName + "] line " + std::to_string(i + 1) + ": Legend marker 'L' must be the start of the line";
				}
				if (templine.length() > MAX_X) {
					return "Error in[" + FileName + "] line " + std::to_string(i + 1) + ": Legend line is too long(max " + std::to_string(MAX_X) + " chars)";
				}
				legendLocation = i;
				roomLegendRows[static_cast<int>(room)] = i;
				i += LEGEND_SIZE-1;
				continue;
			}
		}

		templine.resize(MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		switch (room) {
		case roomIndex::MENU: Menu[i] = templine; break;
		case roomIndex::INSTRUCTIONS: Instructions[i] = templine; break;
		case roomIndex::ROOM1: Room1[i] = templine; break;
		case roomIndex::ROOM2: Room2[i] = templine; break;
		case roomIndex::ROOM3: Room3[i] = templine; break;
		case roomIndex::VAULT: Vault[i] = templine; break;
		case roomIndex::VICTORY: EndingScreen[i] = templine; break;
		}
	}
	inFile.close();
	if (gameRoom) {
		if (legendLocation != -1) {
			try {
				ReadLegendFromFile(room, legendLocation);
			}
			catch (const std::runtime_error& e) {
				return "Error reading legend for [ " + FileName + "]: " + string(e.what());
			}
		}
		else
			return "Error: L (represnet for legend) is missing in [" + FileName + "]";
	}
	return "";
}


void ReadLegendFromFile(roomIndex room, int yOffset) {
	std::ifstream inFile(LegendPathWay);
	if (!inFile.is_open()) {
		throw std::runtime_error("Something wrong with the file Legend.txt");
	}
	string templine;
	for (int i = 0; i < LEGEND_SIZE; ++i) {
		if (!std::getline(inFile, templine)) {
			templine = "";
		}
		templine.resize(MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		int currentLine = yOffset + i;
		if (currentLine >= MAX_Y) {
			continue; // Prevent writing beyond the room's bounds
		}
		switch (room) {
		case roomIndex::ROOM1:
			Room1[currentLine] = templine;
			break;
		case roomIndex::ROOM2:
			Room2[currentLine] = templine;
			break;
		case roomIndex::ROOM3:
			Room3[currentLine] = templine;
			break;
		case roomIndex::VAULT:
			Vault[currentLine] = templine;
			break;
		}
	}
}
