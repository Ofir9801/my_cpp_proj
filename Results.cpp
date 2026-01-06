#include <limits>
#include <fstream>
#include <conio.h>
#include<iostream>
#include "Results.h"
#include "Event.h"

std::string Results::readResultsFromFile(const std::string& filename, Results& outResults)
{
	outResults = Results(); // Reset
	std::ifstream results_file(filename);

	if (!results_file.is_open()) {
		return "Error: Could not open file [" + filename + "]";
	}

	size_t size;
	results_file >> size;
	if (results_file.fail()) return "Error: Invalid format (Size missing)";

	std::string dummy;
	std::getline(results_file, dummy);

	for (size_t i = 0; i < size; i++) {
		size_t iteration;
		int typeInt;
		char player;
		std::string payload;

		results_file >> iteration >> typeInt;
		results_file.get(); 
		results_file.get(player);
		if (results_file.fail()) return "Error: Invalid format at entry " + std::to_string(i);

		std::getline(results_file, payload);
		if (!payload.empty() && payload[0] == ' ') { payload.erase(0, 1); }

		outResults.addResult(Event(iteration, static_cast<EventType>(typeInt), player, payload));
	}
	return ""; // No error
}

Results Results::loadResults(const std::string& filename) {
	Results results;
	std::string errorMsg;

	while (true) {
		errorMsg = readResultsFromFile(filename, results);

		if (errorMsg.empty()) {
			return results; // Success
		}
		system("cls");
		std::cout << "########################################################" << std::endl;
		std::cout << "ERROR LOADING RESULTS FILE" << std::endl;
		std::cout << "########################################################" << std::endl;
		std::cout << errorMsg << std::endl << std::endl;
		std::cout << "1. Fix the file externally." << std::endl;
		std::cout << "2. Press 'r' to RETRY." << std::endl;
		std::cout << "3. Press 'ESC' to EXIT Game." << std::endl;

		char c = static_cast<char>(_getch());
		if (c == ESC) {
			throw std::runtime_error("Game stopped by user due to missing Results file.");
		}
	}
}

void Results::saveResults(const std::string& filename) const {
	std::ofstream results_file(filename);
	results_file << results.size() << std::endl;
	for (const auto& event : results) {
		results_file << event.eventFormat()<<std::endl;
	}
	results_file.close();
}
Event Results::popResult() {
	if (results.empty()) return Event(0, EventType::NO_EVENT, ' ');
	auto result = results.front();
	results.pop_front();
	return result;
}