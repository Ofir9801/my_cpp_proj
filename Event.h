#pragma once
#include <iostream>
#include "utils.h"
class Event{
	EventType type;
	size_t iteration;
	char playerSign;
public:
	Event(size_t iteration, EventType t, char player) :iteration(iteration), type(t), playerSign(player){}
	std::string toString() const;
	size_t getIteration()const { return iteration; }
	EventType getEventType()const { return type; }
	char getPlayer()const { return playerSign; }

};

