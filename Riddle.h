#pragma once
#include "Point.h"
#include "Utils.h"
#include <vector>
#include <string>
class Screen; // Forward declaration
class Player; // Forward declaration

class Riddle
{
	Point position;
	std::string question;
	std::string correctAnswer;
	std::vector<std::string> options;
	int correctIndex;
public:
	Riddle(const Point& pos, std::string q, const std::vector<std::string>& allOptions, int correctIdx) :
		position(pos), question(q), options(allOptions), correctIndex(correctIdx) {
		correctAnswer = options[correctIndex];
		position.setChar(objSigns::RIDDLE);
	}

	Point getPosition() const { return position; }
	bool isAt(const Point& p) const {
		return position == p;
	}
	bool engage(Screen& map, Player& player) const;
};