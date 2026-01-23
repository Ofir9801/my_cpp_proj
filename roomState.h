#pragma once
#include <vector>
#include <map>
#include <string>
#include "Point.h"
#include "Spring.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Door.h"
#include "Key.h"
#include "Riddle.h"
#include "Bomb.h"
struct RoomState {
	bool visited = false;
	std::vector<std::string> layout;
	std::vector<Spring> springs;
	std::vector<Switch> switches;
	std::map<Point, Obstacle> obstacles;
	std::vector<Bomb> activeBombs;
	std::map<Point, Riddle> riddles;
	std::map<Point, Key> keys;
	std::map<int, Door> doors;
};

