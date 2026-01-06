#pragma once
#include <list>
#include <string>
#include "utils.h"

class Results {
	std::list<Event> results; // pair: iteration, result
public:
	static Results loadResults(const std::string& filename);
	void saveResults(const std::string& filename) const;
	void addResult(Event e) {
		results.push_back(e);
	}
	Event popResult() {
		if (results.empty()) return Event( 0, EventType::NO_EVENT,' ');
		auto result = results.front();
		results.pop_front();
		return result;
	}
	bool isFinishedBy(size_t iteration) const {
		return results.empty() || results.back().getIteration()<= iteration;
	}
};
