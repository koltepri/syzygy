#pragma once
#include <raylib.h>
#include "obstacle.h"

class Light
{
public:
    Light(int posX, int posY);
    void Draw(const Obstacle* obstacles, int count) const;

private:    
    int x;
    int y;
    int RayCount;
};