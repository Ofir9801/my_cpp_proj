#pragma once
#include "screen.h"

static const char* room1[screen::MAX_Y] = {
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"player 1:                                                   player 2:           ",  // 0
			"inventory:                                                  inventory:          ",  // 1
			"                                                                                ",  // 2
			"--------------------------------------------------------------------------------",  // 3
			"|                           |           |            |                         |",  // 4
			"|                           |           |            |                         |",  // 5
			"|                           |           |            |                         |",  // 6
			"|                                       |            |                         |",  // 7
			"|                                       |            |                         |",  // 8
			"|                           |           |            |                         |",  // 9
			"|                           |           |            |                         |",  // 10
			"|                           |           |            |                         |",  // 11
			"|                           |           |          ----------------------      |",  // 12
			"|                           |           |                                      |",  // 13
			"|                           |           |                                      |",  // 14
			"|                           |           |                                      |",  // 15
			"|                           |                                                  |",  // 16
			"|                           |                                                  |",  // 17
			"|                           |                                                  |",  // 18
			"|                           |           |                                      |",  // 19
			"|                           |           |                                      |",  // 20
			"|                           |           |                                      |",  // 21
			"|                           |           |                                      |",  // 22
			"|                           |           |                                      |",  // 23
			"--------------------------------------------------------------------------------"   // 24
	};
static const char* menu[screen::MAX_Y] = {
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"						Welcome to the adventure world!                          ",  // 0
			"                                                                                ",  // 1
			"                                                                                ",  // 2
			"                 * To start a new game please enter 1                           ",  // 3
			"                 * To read about instruction and keys please enter 8            ",  // 4
			"                 * To exit the game please enter 9                              ",  // 5
			"                                                                                ",  // 6                                                                                ",  // 1
			"                                                                                ",  // 7
			"                                                                                ",  // 8                                                                                ",  // 1
			"                                                                                ",  // 9
			"                                                                                ",  // 10                                                                                ",  // 1
			"                                                                                ",  // 11
			"                                                                                ",  // 12                                                                                ",  // 1
			"                                                                                ",  // 13
			"                                                                                ",  // 14                                                                                ",  // 1
			"                                                                                ",  // 15
			"                                                                                ",  // 16
			"                                                                                ",  // 17
			"                                                                                ",  // 18
			"                                                                                ",  // 19
			"                                                                                ",  // 20
			"                                                                                ",  // 21
			"                                                                                ",  // 22
			"                                                                                ",  // 23
			"                                                                                "   // 24	
	};
static const char* instructions[screen::MAX_Y] = {
	//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"						Instructions of the game:                                ",  // 0
		"                                                                                ",  // 1
		" *you are trap in world that contains different rooms and you need to get out   ",  // 2
		"                                of there.                                       ",  // 3
		" *between each rooms you have lock door that you need to figure out how to open.",  // 4
		" *you have various objects that locate between the rooms that will help you     ",  // 5
		"     					      unlock the doors                                   ",  // 6
		"                                                                                ",  // 7                                                                                ",  // 1
		"                                                                                ",  // 8
		"                                                                                ",  // 9                                                                                ",  // 1
		"                                                                                ",  // 10
		"                                                                                ",  // 11                                                                                ",  // 1
		"                                                                                ",  // 12
		"                                                                                ",  // 13                                                                                ",  // 1
		"                                                                                ",  // 14
		"                                                                                ",  // 15                                                                                ",  // 1
		"                                                                                ",  // 16
		"                                                                                ",  // 17
		" player 1 key binds:        Player 2 key binds:                                 ",  // 18
		"    UP   -                        UP  -                                         ",  // 19
		"  RIGHT  -                      RIGHT -                                         ",  // 20
		"  DOWN   -                      DOWN  -               Pause the game - ESC OR H ",  // 21
		"  LEFT   -                      LEFT  -                                         ",  // 22
		"  STAY   -                      STAY  -                                         ",  // 23
		" DISPOSE -                    DISPOSE -                                         "   // 24	
};


