#include "screen.h"
#include <iostream>
#include <windows.h>
#include "utils.h"
using std::cout;
using std::endl;

void screen::draw(){
	cls(); //clear the console
	int y = 0;
	for (const char* row : map) {
		gotoxy(0, y++);
		cout << row << std::flush;
	}
	
}





