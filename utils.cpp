#include <iostream>
#include <windows.h> // Required for Windows API console functions
#include <cstdlib>

#include "Utils.h"

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
	else if (isdigit((unsigned char)c))
		return DARKGREY;
	else
		return WHITE;
}