#pragma once
#include "Point.h"
#include "Utils.h"
#include <vector>
#include <string>
class Screen; // Forward declaration
class Player; // Forward declaration

class Riddle
{
	std::string question;
	std::string correctAnswer;
	std::vector<std::string> options;
	int correctIndex;
	bool solved = false;
public:
	Riddle( std::string q, const std::vector<std::string>& allOptions, int correctIdx) :
		 question(q), options(allOptions), correctIndex(correctIdx) {
		correctAnswer = options[correctIndex];
	
	}
	bool engage(Screen& board, Player& player) const;
	bool isSolved() { return solved; }
	void ChangeSolve(bool b) { solved = b; }
};