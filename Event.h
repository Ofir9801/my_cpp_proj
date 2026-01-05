#pragma once
#include <iostream>

class Event

{
public:
	enum EventValue { hitBomb, finished, noResult };
private:
	std::pair<size_t, std::string> action;
	char playerSign;
public:
	Event(size_t iteration, std::string action, char player) :action(iteration,action),playerSign(player){}



};

