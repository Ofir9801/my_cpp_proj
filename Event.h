#pragma once
#include <iostream>
#include "utils.h"
class Event{
	EventType type;
	size_t iteration;
	char playerSign;
	std::string payload;
public:
	Event(size_t iteration, EventType t, char player, std::string extraInfo = "") :iteration(iteration), type(t), playerSign(player), payload(extraInfo) {}
	Event() :iteration(0), type(EventType::NO_EVENT), playerSign(' '), payload("") {}
	size_t getIteration()const { return iteration; }
	EventType getEventType()const { return type; }
	char getPlayer()const { return playerSign; }
	std::string getPayload() const { return payload; }
	std::string eventFormat() const {
		return std::to_string(iteration) + " " + std::to_string(static_cast<int>(type)) + " " + playerSign + " " + payload;
	}
	std::string printEvent()const {
		return "Iteration - " + std::to_string(iteration) + ", Event type - " + std::to_string(static_cast<int>(type)) + ", Player - " + playerSign;
	}

};

