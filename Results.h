#pragma once
#include <list>
#include <string>
#include "utils.h"
#include "Event.h"

class Results {
	std::list<Event> results; // pair: iteration, result
	static std::string readResultsFromFile(const std::string& filename, Results& outResults);
public:
	static Results loadResults(const std::string& filename);
	void saveResults(const std::string& filename) const;
	void addResult(Event e) {results.push_back(e);}
	Event popResult();
	bool isFinishedBy(size_t iteration) const {return results.empty() || results.back().getIteration()<= iteration;}
	bool isEmpty()const { return results.empty(); }
};
