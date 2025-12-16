#pragma once
#include "Point.h"
#include "Utils.h"
#include <vector>
#include <string>
#include "Player.h"

class Riddle
{
	Point position;
	std::string question;
	std::string correctAnswer;
	std::vector<std::string> options;
	int correctIndex;
	bool solved = false;
public:
	Riddle() :position(Point()), question(" "), correctIndex(-1) {}  //default constructor for empty door for map
	Riddle(Point pos) :position(pos), question(" "), correctIndex(-1) {} //default constructor for empty door for map
	Riddle(Point pos, std::string q, const std::vector<std::string>& allOptions, int correctIndex) :
		position(pos), question(q), options(allOptions), correctIndex(correctIndex) {
		if (options.size() > 0 && correctIndex < options.size()) { correctAnswer = options[correctIndex]; }
	}
	bool isSolved() const { return solved; };
	std::string getCorrectAnswer() const { return correctAnswer; };
	int getCorrectIndex() const { return correctIndex; };
	bool engage(Player& player);
};



//void ChangeSolve(bool b) { solved = b; }