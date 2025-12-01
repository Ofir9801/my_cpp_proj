#pragma once


enum INFO_SLOTS {
	PLAYER1_SIGN_START_X = 11,
	PLAYER2_SIGN_START_X = 70,
	PLAYER_SIGN_Y = 0,
	PLAYER1_INV_START_X = 11,
	PLAYER2_INV_START_X = 70,
	PLAYER_INV_Y = 1
};
enum BOARD_DIMENSION{ MAX_X = 80, MAX_Y = 25 };
enum MESSAGES_POS { MES_X = 0, MES_Y = 2 };
enum roomIndex {
	MENU = 0, 
	INSTRUCTIONS = 1,
	ROOM1 = 2,
	ROOM2 = 3 
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
	SWITCH_ON = '/'
};

const char keys1[] = "wdsaeq";
const char keys2[] = "ilkjou";
const char ESC = 27;
int const INVENTORY_SIZE = 1; //Player can hold up to one item
int const NUM_KEYS = 6; //number of possible movement Keys
int constexpr NUM_ROOMS = 4; //number of rooms in the Game


void gotoxy(int x, int y);
void hideCursor();
void cls();

