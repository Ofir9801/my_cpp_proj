#pragma once
#include <string>
#include <iostream>

using std::string;

enum INFO_SLOTS {
	PLAYER1_SIGN_START_X = 10,
	PLAYER1_INV_START_X = 10,
	PLAYER2_SIGN_START_X = 74,
	PLAYER2_INV_START_X = 74,
	SCORE_START_X = 33,
	LIVES_START_X = 49,
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
	ROOM3 = 4,
	VICTORY = 5
};
enum Keyboard_bind {
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
	TORCH = '!',
	SPRING = '#',
	EMPTY = ' ',
	RIDDLE = '?',
	SWITCH_OFF = '\\',
	SWITCH_ON = '/',
	BOMB = '@'
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

const char ESC = 27;
const string EMPTYLINE = "                                                                                ";
const string keys1 = "wdsaeq";
const string keys2 = "ilkjou";

inline string Room1[BOARD_DIMENSION::MAX_Y];
inline string Room2[BOARD_DIMENSION::MAX_Y];
inline string Room3[BOARD_DIMENSION::MAX_Y];
inline string Menu[BOARD_DIMENSION::MAX_Y];
inline string Instructions[BOARD_DIMENSION::MAX_Y];
inline string EndingScreen[BOARD_DIMENSION::MAX_Y];

const string MenuPathWay = "Rooms/Menu.txt";
const string Room1PathWay = "Rooms/Room1.txt";
const string Room2PathWay = "Rooms/Room2.txt";
const string Room3PathWay = "Rooms/Room3.txt";
const string EndingScreenPathWay = "Rooms/EndingScreen.txt";
const string InstructionsPathWay = "Rooms/Instructions.txt";
const string RiddlePathWay = "Riddle.txt";


int constexpr INVENTORY_SIZE = 1; //Player can hold up to one item
int constexpr NUM_KEYS = 6; //number of possible movement Keys
int constexpr NUM_ROOMS = 6; //number of rooms in the Game
int constexpr LIGHT_RADIUS_TORCH = 7;
int constexpr LIGHT_RADIUS_DEFAULT = 2;
int constexpr NUMS_OF_RIDDLES = 4;
int constexpr EXPLODE_BOMB_TIME = 15;
int constexpr BOMB_RADIUS = 3;
int constexpr PLAYER_1_START_X = 1;
int constexpr PLAYER_1_START_Y = 4;
int constexpr PLAYER_2_START_X = 75;
int constexpr PLAYER_2_START_Y = 4;
int constexpr MIN_SCORE = 200

void gotoxy(int x, int y);
void hideCursor();
void cls();
void SetTextColor(int color);
int getColorForChar(char c);
bool ReadRoomLayoutFromFile(string FileName, int roomIndex);

