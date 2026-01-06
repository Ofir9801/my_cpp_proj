#include "Event.h"

std::string Event::toString() const
{
    std::string typeStr;
    switch (type) {
    case EventType::DOOR_OPEN: typeStr = "DOOR_OPEN"; break;
    case EventType::BOMB_EXPLODE: typeStr = "BOMB_EXPLODE"; break;
    case EventType::RIDDLE_SOLVED: typeStr = "RIDDLE_SOLVED"; break;
    case EventType::ROOM_CHANGE: typeStr = "ROOM_CHANGE"; break;
    case EventType::GAME_OVER: typeStr = "GAME_OVER"; break;
    default:typeStr = "";
        return typeStr;
    }
}
