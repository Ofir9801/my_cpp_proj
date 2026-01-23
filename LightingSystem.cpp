#include "LightingSystem.h"
#include "screen.h"
#include "player.h"
#include <cmath>
#include <iostream>

using std::cout;

bool LightingSystem::roundDistance(int x, int y, const Point& p, int radius) const
{
	int dx = x - p.getX(); //calculate the distance between two points by x
	int dy = y - p.getY(); //calculate the distance between two points by y
	return (dx * dx + dy * dy) <= (radius * radius); //true if the given point is in player's light radius
}

void LightingSystem::ProcessArea(int cx, int cy, int radius, bool erase, const Point& p1, const Point& p2, int r1, int r2)
{
	int legendY = screenContext->getLegendY();
	for (int y = cy - radius; y <= cy + radius; y++) {
		for (int x = cx - radius; x <= cx + radius; x++) {
			
			//skip check in board limits
			if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y)
				continue;

			//skip legend area
			if (screenContext->inLegendBounds(legendY, y)) {
				continue;
			}
			bool inRange = roundDistance(x, y, Point(cx, cy), radius); //check if point in distance

			if (inRange) {
				if (erase) {
					if (!roundDistance(x, y, p1, r1) && !roundDistance(x, y, p2, r2)) {
						gotoxy(x, y);
						cout << ' ';
					}
				}
				else {
					if ((x == p1.getX() && y == p1.getY()) || (x == p2.getX() && y == p2.getY())) {
						continue;
					}
					gotoxy(x, y);
					char c = screenContext->getCharAt(Point(x,y));
					if (screenContext->IsColor()) {
						SetTextColor(getColorForChar(c));
						cout << c;
						SetTextColor(Color::WHITE);
					}
					else {
						cout << c;
					}
				}
			}
		}
	}

}

void LightingSystem::updateLighting(const Point& p1, const Point& p1Prev, const Player& player1,const Point& p2, const Point& p2Prev, const Player& player2){

	int r1 = player1.hasItem(objSigns::TORCH) ? LIGHT_RADIUS_TORCH : LIGHT_RADIUS_DEFAULT;
	int r2 = player2.hasItem(objSigns::TORCH) ? LIGHT_RADIUS_TORCH : LIGHT_RADIUS_DEFAULT;

	//erase old area
	ProcessArea(p1Prev.getX(), p1Prev.getY(), LIGHT_RADIUS_TORCH, true, p1, p2, r1, r2);
	ProcessArea(p2Prev.getX(), p2Prev.getY(), LIGHT_RADIUS_TORCH, true, p1, p2, r1, r2);

	ProcessArea(p1.getX(), p1.getY(), r1, false, p1, p2, r1, r2);
	ProcessArea(p2.getX(), p2.getY(), r2, false, p1, p2, r1, r2);
}