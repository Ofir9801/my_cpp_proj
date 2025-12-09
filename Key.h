#pragma once
#include "Point.h"

class Screen;//forward declaration to avoid circular dependency

class Key {
    Point position;
    //Screen* map;
    int targetDoorId = -1;
    bool inUse = false;

public:
    Key(int x, int y, int targetDoorId = -1)
        : position(x, y, objSigns::KEY), targetDoorId(targetDoorId) {
    }   

    void setTargetDoorId(int doorId) { targetDoorId = doorId; }
    int getTargetDoorId() const { return targetDoorId; }
    
    void setUse(bool use) { inUse = use; }
    bool getUse() const { return inUse; }
    Point getPosition() const { return position; }
};