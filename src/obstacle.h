#pragma once
#include <raylib.h>

enum ObstacleType { KASSI, CIRCLE };

struct Obstacle
{
    ObstacleType    type;
    Rectangle       rect;
    Vector2         center;    //Kúlu miðju hnit
    float           radius;
    bool            reflective;
    Color           color;
    float           rotation;
};