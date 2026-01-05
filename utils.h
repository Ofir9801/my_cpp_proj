#pragma once
#include <string>
#include <iostream>
#include <vector>

using std::string;

enum class INFO_SLOTS {//for players' display info
	PLAYER1_SIGN_START_X = 10,
	PLAYER1_INV_START_X = 10,
	PLAYER2_SIGN_START_X = 74,
	PLAYER2_INV_START_X = 74,
	SCORE_START_X = 49,
	LIVES_START_X = 33,
	PLAYER_SIGN_Y = 0,
	PLAYER_INV_Y = 1
};
enum class MESSAGES_POS {//for message display area
	MES_X = 0,
	MES_Y = 2 
};
enum class roomIndex {//rooms indices
	VAULT = 0,
	ROOM1 = 1,
	ROOM2 = 2,
	ROOM3 = 3,
	VICTORY = 4,
	MENU = 5,
	INSTRUCTIONS = 6,
};
enum class Keyboard_bind {//possible movement directions for players
	UP,
	RIGHT,
	DOWN,
	LEFT,
	STAY,
	DISPOSE
};
enum class objSigns {//game objects' signs on the board
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
enum class Color {//when colors are enabled, this is how we access colors
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
//constants of basic game handling
inline constexpr char ESC = 27;
inline const string EMPTYLINE = "                                                                                ";
inline const string keys1 = "wdsaeq";
inline const string keys2 = "ilkjou";

//constants of game design
inline constexpr int MAX_X = 80;
inline constexpr int MAX_Y = 25;
inline constexpr int INVENTORY_SIZE = 2; //Player can hold up to one item
inline constexpr int NUM_KEYS = 6; //number of possible movement Keys
inline constexpr int NUM_ROOMS = 7; //number of rooms in the Game
inline constexpr int LIGHT_RADIUS_TORCH = 7;
inline constexpr int LIGHT_RADIUS_DEFAULT = 2;
inline constexpr int EXPLODE_BOMB_TIME = 15;
inline constexpr int BOMB_RADIUS = 2;
inline constexpr int BOMB_WALL_EFFECTIVE_RADIUS = 1;
inline constexpr int PLAYER_1_START_X = 1;
inline constexpr int PLAYER_1_START_Y = 4;
inline constexpr int PLAYER_2_START_X = 75;
inline constexpr int PLAYER_2_START_Y = 4;
inline constexpr int MIN_SCORE = 500;
inline constexpr int FAKE_DOOR_SCORE = 50;
inline constexpr int WRONG_ANSWER = 50;
inline constexpr int SUCCESS_SCORE = 100;
inline constexpr int STARTING_LIVES = 4;
inline int constexpr TIMER_MESSAGE = 15;
inline int constexpr LEGEND_SIZE = 3;
inline int roomLegendRows[NUM_ROOMS];

inline string Room1[MAX_Y];
inline string Room2[MAX_Y];
inline string Room3[MAX_Y];
inline string Vault[MAX_Y];
inline string EndingScreen[MAX_Y];
inline string Menu[MAX_Y];
inline string Instructions[MAX_Y];

inline const string MenuPrefix = "Menu";
inline const string InstructionsPrefix = "Instructions";
inline const string VictoryPrefix = "Final";
inline const string VaultPrefix = "Vault";
inline const string RoomsFolder = "Rooms";
inline const string RiddlesFolder = "Riddles";
inline const string RoomsExtension = ".screen";
inline const string RiddlesExtension = ".riddle";
inline const string LegendPathWay = "Rooms/Legend.legend";

//utility functions for screen handling
void gotoxy(int x, int y);
void hideCursor();
void cls();
//color functions
void SetTextColor(Color color);
Color getColorForChar(char c);
Color getColorForChar(objSigns sign);
//file reading functions
void ReadLegendFromFile(roomIndex room, int yOffset);
string ReadRoomFromFile();
roomIndex getRoomNumber(std::string fileName);
void getAllFilePaths(std::vector<std::string>& vec_to_fill, std::string extension, std::string subFolder="");
void WriteLineToRoom(roomIndex room, int lineIndex, const std::string& text);

inline void gotoxy(INFO_SLOTS x, INFO_SLOTS y) { gotoxy(static_cast<int>(x), static_cast<int>(y)); }
inline void gotoxy(INFO_SLOTS x, int y) { gotoxy(static_cast<int>(x), y); }
inline void gotoxy(MESSAGES_POS x, int y) { gotoxy(static_cast<int>(x), y); }
inline bool isGameRoom(roomIndex room) {
	return room == roomIndex::ROOM1 || room == roomIndex::ROOM2 || room == roomIndex::ROOM3 || room == roomIndex::VAULT;
}
//operator overloads for enum classes
inline bool operator==(char c, objSigns sign) { //overload operator == for enum class objsign
	return c == static_cast<char>(sign);
}
inline bool operator==(int n, roomIndex index) { //overload operator == for enum class roomIndex
	return n == static_cast<int>(index);
}
inline bool operator==(roomIndex index, int n) { //overload operator == for enum class roomIndex
	return static_cast<int>(index) == n;
}
inline bool operator>= (int n, roomIndex index) { //overload operator >= for enum class roomIndex
	return n >= static_cast<int>(index);
}
inline bool operator<= (int n, roomIndex index) { //overload operator <= for enum class roomIndex
	return n <= static_cast<int>(index);
}
inline bool operator< (int n, roomIndex index) { //overload operator < for enum class roomIndex
	return n < static_cast<int>(index);
}
inline bool operator> (int n, roomIndex index) { //overload operator > for enum class roomIndex
	return n > static_cast<int>(index);
}
inline std::ostream& operator<<(std::ostream& os, const objSigns& sign) { //overload operator << for enum class objsign
	os << static_cast<char>(sign);
	return os;
}
inline int operator+ (MESSAGES_POS p, int offset) { //overload operator <= for enum class roomIndex
	return static_cast<int>(p) + offset;
}
inline int operator+ (int offset, MESSAGES_POS p) { //overload operator <= for enum class roomIndex
	return offset + static_cast<int>(p);
}
