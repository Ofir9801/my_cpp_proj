#include <limits>
#include <fstream>

#include "Results.h"
#include "Event.h"

Results Results::loadResults(const std::string& filename) {
	std::ifstream results_file(filename);
	Results results;
	if (!results_file.is_open()) { return results; } // should alert the user! FLAG	

	size_t size;
	results_file >> size;
	std::string dummy;
	std::getline(results_file, dummy);
	for (size_t i = 0; i < size; i++) {
		size_t iteration;
		int typeInt;
		char player;
		std::string payload;

		results_file >> iteration >> typeInt >> player;
		std::getline(results_file, payload);
		if (!payload.empty() && payload[0] == ' ') { payload.erase(0, 1); }
		results.addResult(Event(iteration, static_cast<EventType>(typeInt), player,payload));
	}
	return results;
}

void Results::saveResults(const std::string& filename) const {
	std::ofstream results_file(filename);
	results_file << results.size() << std::endl;
	for (const auto& event : results) {
		results_file << event.eventFormat();
	}
	results_file.close();
}
Event Results::popResult() {
	if (results.empty()) return Event(0, EventType::NO_EVENT, ' ');
	auto result = results.front();
	results.pop_front();
	return result;
}