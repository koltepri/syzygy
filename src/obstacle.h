#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <raylib.h>

typedef enum { KASSI, CIRCLE } ObstacleType;

typedef struct
{
    ObstacleType    type;
    Rectangle       rect;
    Vector2         center;
    float           radius;
    bool            reflective;
    Color           color;
    float           rotation;
} Obstacle;

#endif
