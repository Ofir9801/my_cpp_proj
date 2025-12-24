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

void gotoxy(INFO_SLOTS x, INFO_SLOTS y)
{
	std::cout.flush();
	COORD coord;
	coord.X = (int)x;
	coord.Y = (int)y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void gotoxy(MESSAGES_POS x, MESSAGES_POS y)
{
	std::cout.flush();
	COORD coord;
	coord.X = (int)x;
	coord.Y = (int)y;
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

bool ReadRoomLayoutFromFile(string FileName, roomIndex roomIndex) {
	std::ifstream inFile(FileName);
	bool error = false;
	if (!inFile.is_open()) {
		error = true;
	}
	string templine;
	for (int i = 0; i < MAX_Y; i++) {
		if (!std::getline(inFile, templine)) {
			templine = "";
		}
		templine.resize(MAX_X, ' ');//if the line is bigger then MAX_X, it truncates it, if the line is shorter then 80, it add space bars to fill the missing places
		switch (roomIndex) {
		case roomIndex::MENU:
			Menu[i] = templine;
			break;
		case roomIndex::INSTRUCTIONS:
			Instructions[i] = templine;
			break;
		case roomIndex::ROOM1:
			Room1[i] = templine;
			break;
		case roomIndex::ROOM2:
			Room2[i] = templine;
			break;
		case roomIndex::ROOM3:
			Room3[i] = templine;
			break;
		case roomIndex::VICTORY:
			EndingScreen[i] = templine;
			break;

		}
	}
	inFile.close();
	return error;
}