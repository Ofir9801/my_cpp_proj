#pragma once
#include "Point.h"
#include "Utils.h"
class Screen;
class Player;

class LightingSystem
{
	Screen* screenContext;
	bool isDarkRoom = false;
	void ProcessArea(int cx, int cy, int radius, bool erase, const Point& p1, const Point& p2, int r1, int r2);
public:
	LightingSystem(Screen* screen) : screenContext(screen) {}
	void setDarkRoom(bool isDark) { isDarkRoom = isDark; }
	bool IsDark() const { return isDarkRoom; }
	void updateLighting(const Point& p1, const Point& p1Prev, const Player& player1,
		const Point& p2, const Point& p2Prev, const Player& player2);
	bool roundDistance(int x, int y, const Point& p, int radius) const;
};

