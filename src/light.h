#ifndef LIGHT_H // DIFF: replaced #pragma once
#define LIGHT_H

#include "obstacle_c.h"
#include <raylib.h>

typedef struct {
  int x;
  int y;
  int RayCount;
} Light;

Light Light_Create(int posX, int posY);

void Light_Draw(const Light *light, const Obstacle *obstacles, int count);

#endif
