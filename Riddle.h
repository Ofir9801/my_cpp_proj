#pragma once
#include "point.h"
#include <string>
class Screen; // Forward declaration
class Player; // Forward declaration

class Riddle
{
	point position;
	std::string question;
	char answer;
public:
	Riddle(const point& pos, const std::string& ques, char ans) : position(pos), question(ques), answer(ans) {
		position.setChar('?');
	}
	point getPosition() const { return position; }
	bool isAt(const point& p) const {
		return position.getX() == p.getX() && position.getY() == p.getY();
	}
	bool engage(Screen& map, Player& player) const;
};