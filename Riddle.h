#pragma once
#include "Point.h"
#include "Utils.h"
#include <vector>
#include <string>
#include "Player.h"

class Riddle

{//Riddle class to represent riddles on the board, quite self explanatory
	Point position;
	std::string question;
	std::string correctAnswer;
	std::vector<std::string> options;
	int correctIndex;
	bool solved = false;
public:
	Riddle() :position(Point()), question(" "), correctIndex(-1) {}  //default constructor for empty door for map
	Riddle(Point pos) :position(pos), question(" "), correctIndex(-1) {}
	Riddle(const Point& pos, std::string question, std::string correctAnswer) :position(pos), question(question), correctAnswer(correctAnswer), correctIndex(-1) {}
	Riddle(Point pos, std::string q, const std::vector<std::string>& allOptions, int correctIndex) :
		position(pos), question(q), options(allOptions), correctIndex(correctIndex) {
		if (options.size() > 0 && correctIndex < options.size()) { correctAnswer = options[correctIndex]; }
	}
	bool isSolved() const { return solved; };
	std::string getCorrectAnswer() const { return correctAnswer; };
	int getCorrectIndex() const { return correctIndex; };
	bool engage(Player& player, std::string& answer);
	bool engageVaultRiddle(std::string& str);//vault riddle has no options, player must type the answer
	void save(std::ofstream& file) const;
	void load(std::ifstream& file);
};