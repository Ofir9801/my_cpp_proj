#pragma once
#include <string>
#include <iostream>

using std::string;

enum INFO_SLOTS {
	PLAYER1_SIGN_START_X = 11,
	PLAYER1_LIVES_START_X = 24,
	PLAYER1_SCORE_START_X = 24,
	PLAYER1_INV_START_X = 11,

	PLAYER2_SIGN_START_X = 63,
	PLAYER2_LIVES_START_X = 74,
	PLAYER2_SCORE_START_X = 74,
	PLAYER2_INV_START_X = 63,

	PLAYER_SIGN_Y = 0,
	PLAYER_INV_Y = 1
};
enum BOARD_DIMENSION{ MAX_X = 80, MAX_Y = 25 };
enum MESSAGES_POS { MES_X = 0, MES_Y = 2 };
enum roomIndex {
	MENU = 0,
	INSTRUCTIONS = 1,
	ROOM1 = 2,
	ROOM2 = 3,
	VICTORY = 4
};
enum Keys {
	UP,
	RIGHT,
	DOWN,
	LEFT,
	STAY,
	DISPOSE
};
enum objSigns {
	PLAYER1 = '$',
	PLAYER2 = '&',
	KEY = 'K',
	OBSTACLE = '*',
	SPRING = '#',
	DOOR = '1',
	EMPTY = ' ',
	SWITCH_OFF = '\\',
	SWITCH_ON = '/',
	BOMB = '@',
};
enum Color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHTGREY = 7,
	DARKGREY = 8,
	LIGHTBLUE = 9,
	LIGHTGREEN = 10,
	LIGHTCYAN = 11,
	LIGHTRED = 12,
	LIGHTMAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};

const string EMPTYLINE = "                                                                                ";
const string keys1 = "wdsaeq";
const string keys2 = "ilkjou";
const char ESC = 27;
inline string Room1[BOARD_DIMENSION::MAX_Y];
inline string Room2[BOARD_DIMENSION::MAX_Y];
inline string Menu[BOARD_DIMENSION::MAX_Y];
inline string Instructions[BOARD_DIMENSION::MAX_Y];
inline string EndingScreen[BOARD_DIMENSION::MAX_Y];

int constexpr INVENTORY_SIZE = 1; //Player can hold up to one item
int constexpr NUM_KEYS = 6; //number of possible movement Keys
int constexpr NUM_ROOMS = 5; //number of rooms in the Game

void gotoxy(int x, int y);
void hideCursor();
void cls();
void SetTextColor(int color);
int getColorForChar(char c);
void ReadRoomLayoutFromFile(string FileName, int roomIndex);

