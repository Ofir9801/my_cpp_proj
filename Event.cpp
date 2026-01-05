#include "Event.h"

std::string Event::toString() const
{
    std::string typeStr;
    switch (type) {
    case EventType::DOOR_OPEN: typeStr = "DOOR_OPEN"; break;
    case EventType::BOMB_EXPLODE: typeStr = "BOMB_EXPLODE"; break;
    case EventType::RIDDLE_SOLVED: typeStr = "RIDDLE_SOLVED"; break;
    case EventType::LEVEL_FINISHED: typeStr = "LEVEL_FINISHED"; break;
    case EventType::GAME_OVER: typeStr = "GAME_OVER"; break;
    }
}
