#pragma once
#include <string>
#include <iostream>

using std::string;

enum class INFO_SLOTS {
	PLAYER1_SIGN_START_X = 10,
	PLAYER1_INV_START_X = 10,
	PLAYER2_SIGN_START_X = 74,
	PLAYER2_INV_START_X = 74,
	SCORE_START_X = 49,
	LIVES_START_X = 33,
	PLAYER_SIGN_Y = 0,
	PLAYER_INV_Y = 1
};
enum class MESSAGES_POS {
	MES_X = 0,
	MES_Y = 2 
};
enum class roomIndex {
	MENU = 0,
	INSTRUCTIONS = 1,
	ROOM1 = 2,
	ROOM2 = 3,
	ROOM3 = 4,
	VICTORY = 5
};
enum class Keyboard_bind {
	UP,
	RIGHT,
	DOWN,
	LEFT,
	STAY,
	DISPOSE
};
enum class objSigns {
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
enum class Color {
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

inline constexpr char ESC = 27;
inline const string EMPTYLINE = "                                                                                ";
inline const string keys1 = "wdsaeq";
inline const string keys2 = "ilkjou";
inline constexpr int MAX_X = 80;
inline constexpr int MAX_Y = 25;

inline string Room1[MAX_Y];
inline string Room2[MAX_Y];
inline string Room3[MAX_Y];
inline string Menu[MAX_Y];
inline string Instructions[MAX_Y];
inline string EndingScreen[MAX_Y];

inline const string MenuPathWay = "Rooms/Menu.txt";
inline const string Room1PathWay = "Rooms/Room1.txt";
inline const string Room2PathWay = "Rooms/Room2.txt";
inline const string Room3PathWay = "Rooms/Room3.txt";
inline const string EndingScreenPathWay = "Rooms/EndingScreen.txt";
inline const string InstructionsPathWay = "Rooms/Instructions.txt";
inline const string RiddlePathWay = "Riddle.txt";

inline constexpr int INVENTORY_SIZE = 1; //Player can hold up to one item
inline constexpr int NUM_KEYS = 6; //number of possible movement Keys
inline constexpr int NUM_ROOMS = 6; //number of rooms in the Game
inline constexpr int LIGHT_RADIUS_TORCH = 7;
inline constexpr int LIGHT_RADIUS_DEFAULT = 2;
inline constexpr int NUMS_OF_RIDDLES = 4;
inline constexpr int EXPLODE_BOMB_TIME = 15;
inline constexpr int BOMB_RADIUS = 3;
inline constexpr int PLAYER_1_START_X = 1;
inline constexpr int PLAYER_1_START_Y = 4;
inline constexpr int PLAYER_2_START_X = 75;
inline constexpr int PLAYER_2_START_Y = 4;
inline constexpr int MIN_SCORE = 300;
inline constexpr int FAKE_DOOR_SCORE = 50;
inline constexpr int WRONG_ANSWER = 50;
inline constexpr int SUCCESS_SCORE = 100;
inline constexpr int STARTING_LIVES = 4;


//add score points to fake door and winning door

void gotoxy(int x, int y);
void gotoxy(INFO_SLOTS x, INFO_SLOTS y);
void gotoxy(MESSAGES_POS x, MESSAGES_POS y);
void hideCursor();
void cls();
void SetTextColor(Color color);
Color getColorForChar(char c);
bool ReadRoomLayoutFromFile(string FileName, roomIndex roomIndex);

inline bool operator==(char c, objSigns sign) { //overload operator == for enum class objsign
	return c == static_cast<char>(sign);
}
inline bool operator==(int n, roomIndex index) { //overload operator 
	return n == static_cast<int>(index);
}