#pragma once
#include "Point.h"
class Screen;//forward declaration to avoid circular dependency

class Key {
    Point position;
    int targetDoorId = -1;
    char playerUse = ' ' ; //the id of the player who use the key,' ' = not in use

public:
    Key() : position(0, 0, ' '), targetDoorId(-1), playerUse(' ') {} //default constructor for empty key for map

    Key(int x, int y, int targetDoorId = -1, char playerUse = ' ')
        : position(x, y, objSigns::KEY), targetDoorId(targetDoorId), playerUse(playerUse) {
    }   

    void setTargetDoorId(int doorId) { targetDoorId = doorId; }
    int getTargetDoorId() const { return targetDoorId; }
    void setInUse(char id) {playerUse = id; }
    char getInUse() const { return playerUse; }
    char getPlayerUse() const { return playerUse; }
    Point getPosition() const { return position; }
    void SetPosition(Point newPos){
        position.setX(newPos.getX());
        position.setY(newPos.getY());
    }
};