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

void SetTextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int getColorForChar(char c)
{
	if (c == objSigns::PLAYER1 || c == objSigns::PLAYER2)
		return CYAN;
	else if (c == objSigns::KEY)
		return YELLOW;
	else if (c == objSigns::OBSTACLE)
		return BROWN;
	else if (c == objSigns::SPRING)
		return MAGENTA;
	else if (c == objSigns::SWITCH_ON)
		return GREEN;
	else if (c == objSigns::SWITCH_OFF)
		return RED;
	else if (c == objSigns::BOMB)
		return LIGHTRED;
	else if (c == objSigns::RIDDLE)
		return LIGHTGREEN;
	else if (c == objSigns::TORCH)
		return YELLOW;
	else if (isdigit((unsigned char)c))
		return DARKGREY;
	
	else
		return WHITE;
}

bool ReadRoomLayoutFromFile(string FileName, int roomIndex) {
	std::ifstream inFile(FileName);
	bool error = false;
	if (!inFile.is_open()) {
		error = true;
	}
	string templine;
	for (int i = 0; i < BOARD_DIMENSION::MAX_Y; ++i) {
		if (!std::getline(inFile, templine)) {
			templine = "";
		}
		templine.resize(BOARD_DIMENSION::MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		switch (roomIndex) {
		case roomIndex::MENU:
			Menu[i] = templine;
			break;
		case roomIndex::INSTRUCTIONS:
			Instructions[i] = templine;
			break;
		case roomIndex::ROOM1:
			if (i == 0) {error = HandleLegendLine(templine, roomIndex, i);}
			else {Room1[i] = templine;}
			break;
		case roomIndex::ROOM2:
			if (i == 0) { error = HandleLegendLine(templine, roomIndex, i); }
			else {Room2[i] = templine;}
			break;
		case roomIndex::ROOM3:
			if (i == 0) { error = HandleLegendLine(templine, roomIndex, i); }
			else {Room3[i] = templine;}
			break;
		case roomIndex::VAULT:
			if (i == 0) { error = HandleLegendLine(templine, roomIndex, i); }
			else {Vault[i] = templine;}
			break;		
		case roomIndex::VICTORY:
			EndingScreen[i] = templine;
			break;
		}
	}
	inFile.close();
	return error;
}

bool HandleLegendLine(string& line, int roomIndex, int& loopIndex) {
	if (!line.empty() && line[0] == 'L')
	{
		ReadLegendFromFile(roomIndex);
		loopIndex = 2; //skip next two lines since they are part of the legend
		return false;
	}
	return true;
}

void ReadLegendFromFile(int roomIndex) {
	std::ifstream inFile(LegendPathWay);
	if (!inFile.is_open()) {
		throw std::runtime_error("Something wrong with the file Legend.txt");
	}
	string templine;
	for (int i = 0; i < LEGEND_SIZE;++i) {
		if (!std::getline(inFile, templine)) {
			templine = "";
		}
		templine.resize(BOARD_DIMENSION::MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		switch (roomIndex) {
		case roomIndex::ROOM1:
			Room1[i] = templine;
			break;
		case roomIndex::ROOM2:
			Room2[i] = templine;
			break;
		case roomIndex::ROOM3:
			Room3[i] = templine;
			break;
		case roomIndex::VAULT:
			Vault[i] = templine;
			break;
		}
	}
	inFile.close();
}