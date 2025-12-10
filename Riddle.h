#pragma once
#include "Point.h"
#include <string>
class Screen; // Forward declaration
class Player; // Forward declaration

class Riddle
{
	Point position;
	std::string question;
	char answer;
public:
	Riddle(const Point& pos, const std::string& ques, char ans) : position(pos), question(ques), answer(ans) {
		position.setChar('?');
	}
	Point getPosition() const { return position; }
	bool isAt(const Point& p) const {
		return position == p;
	}
	bool engage(Screen& map, Player& player) const;
};