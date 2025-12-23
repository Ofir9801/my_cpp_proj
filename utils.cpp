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
		return (int)Color::CYAN;
	else if (c == objSigns::KEY)
		return (int)Color::YELLOW;
	else if (c == objSigns::OBSTACLE)
		return (int)Color::BROWN;
	else if (c == objSigns::SPRING)
		return (int)Color::MAGENTA;
	else if (c == objSigns::SWITCH_ON)
		return (int)Color::GREEN;
	else if (c == objSigns::SWITCH_OFF)
		return (int)Color::RED;
	else if (c == objSigns::BOMB)
		return (int)Color::LIGHTRED;
	else if (c == objSigns::RIDDLE)
		return (int)Color::LIGHTGREEN;
	else if (c == objSigns::TORCH)
		return (int)Color::YELLOW;
	else if (isdigit((unsigned char)c))
		return (int)Color::DARKGREY;
	
	else
		return (int)Color::WHITE;
}

bool ReadRoomLayoutFromFile(string FileName, int roomIndex) {
	std::ifstream inFile(FileName);
	bool error = false;
	if (!inFile.is_open()) {
		error = true;
	}
	string templine;
	for (int i = 0; i < BOARD_DIMENSION::MAX_Y; i++) {
		if (!std::getline(inFile, templine)) {
			templine = "";
		}
		templine.resize(BOARD_DIMENSION::MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		switch (roomIndex) {
		case (int)roomIndex::MENU:
			Menu[i] = templine;
			break;
		case (int)roomIndex::INSTRUCTIONS:
			Instructions[i] = templine;
			break;
		case (int)roomIndex::ROOM1:
			Room1[i] = templine;
			break;
		case (int)roomIndex::ROOM2:
			Room2[i] = templine;
			break;
		case (int)roomIndex::ROOM3:
			Room3[i] = templine;
			break;
		case (int)roomIndex::VICTORY:
			EndingScreen[i] = templine;
			break;

		}
	}
	inFile.close();
	return error;
}