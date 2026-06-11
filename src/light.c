#include "light.h"
#include "obstacle.h"
#include <math.h>    
#include <stdbool.h>
static bool PointInsideObject(Vector2 point,
                              const Obstacle *obstacle) { // DIFF: & → *
  if (obstacle->type ==KASSI) 
    return CheckCollisionPointRec(point, obstacle->rect);
  else
    return CheckCollisionPointCircle(point, obstacle->center, obstacle->radius);}


static Vector2 NormVector(Vector2 point, const Obstacle *object) {
  if (object->type == CIRCLE) { 
    Vector2 ab = {point.x - object->center.x, point.y - object->center.y};
    float c = sqrtf(ab.x * ab.x + ab.y * ab.y);
    return (Vector2){ab.x / c, ab.y /c};}

  if (object->type == KASSI) {
    float left = point.x - object->rect.x;
    float right = (object->rect.x + object->rect.width) - point.x;
    float top = point.y - object->rect.y;
    float bottom = (object->rect.y + object->rect.height) - point.y;

    float nearest = fminf(fminf(left, right), fminf(top, bottom));
    if (nearest == left) return (Vector2){-1, 0};
    if (nearest == right)return (Vector2){1, 0};
    if (nearest == top)return (Vector2){0,-1}; // raylib https://i1.sndcdn.com/artworks-000394759668-wan0rk-t1080x1080.jpg
    else return (Vector2){0, 1};} 
  return (Vector2){0, 0};}

Light Light_Create(int posX, int posY) {
  Light light;    
  light.x = posX; 
  light.y = posY;
  light.RayCount = 180; //TODO Dynamic fjöldi geisla, fleirri nálægt endum á objects, færri í miðjunni og á svæðum sem það eru engir objects, lélegt computation
  return light;
}


void Light_Draw(const Light *light, const Obstacle *obstacles,int count) {
  int MaxBounces = 5;
  int ScreenHeight = GetScreenHeight();
  int ScreenWidth = GetScreenWidth();

  for (int i = 0; i < light->RayCount;i++) {
    float angle = (i * 360.0f / light->RayCount) * DEG2RAD;
    float dx = cosf(angle);
    float dy = sinf(angle);

    float Walking_x = (float)light->x; 
    float Walking_y = (float)light->y;
    float StartX = Walking_x;
    float StartY = Walking_y;
    float step = 3.0;
    int bounces = 0;

    while (1) { 
      Walking_x += dx * step;
      Walking_y += dy * step;

      if (Walking_x < 0 || Walking_x > ScreenWidth || Walking_y < 0 ||
          Walking_y > ScreenHeight) {
        DrawLine(StartX, StartY, Walking_x, Walking_y, Fade(YELLOW, 0.3f));
        break;
      }
      if (sqrtf((Walking_x - light->x)*(Walking_x - light->x)+
                (Walking_y - light->y)*(Walking_y - light->y)) > 400) {
        DrawLine(StartX, StartY, Walking_x, Walking_y, Fade(YELLOW, 0.3f));
        break;}

      int hit = -1;
      for (int j = 0; j < count; j++) {
        if (PointInsideObject((Vector2){Walking_x, Walking_y}, &obstacles[j])==true) { 
          hit = j;
          break;
        }
      }

      if (hit >= 0) {
        const Obstacle *object = &obstacles[hit];
        DrawLine(StartX, StartY, Walking_x, Walking_y, Fade(YELLOW, 0.3f));

        if (object->reflective && bounces < MaxBounces) {
          Vector2 norm =
              NormVector((Vector2){Walking_x, Walking_y},object);
          float DotProduct = dx * norm.x + dy * norm.y;
          dx = dx - 2 * DotProduct * norm.x;
          dy = dy - 2 * DotProduct * norm.y;

          Walking_x += dx * step * 2;
          Walking_y += dy * step * 2;
          StartX = Walking_x;
          StartY = Walking_y;
          bounces++;
          continue;
        }

        break;
      }
    }
  }
}


void Shadow_Removal (const Light *light, const Obstacle *obstacles, int count) { //Lazy spaghetti kóði, örugglega hægt að sameina við light_draw
  int MaxBounces = 5;
  int ScreenHeight = GetScreenHeight();
  int ScreenWidth = GetScreenWidth();
  float thickness = (2.0f * PI * 600.0f) / light->RayCount * 1.5f; //Thicc
  for (int i = 0; i < light->RayCount; i++) {
    float angle = (i * 360.0f / light->RayCount) * DEG2RAD;
    float dx = cosf(angle);
    float dy = sinf(angle);
    float Walking_x = (float)light->x;
    float Walking_y = (float)light->y;
    float StartX = Walking_x;
    float StartY = Walking_y;
    float step = 3.0;
    int bounces = 0;
    while (1) {
      Walking_x += dx * step;
      Walking_y += dy * step;
      if (Walking_x < 0 || Walking_x > ScreenWidth || Walking_y < 0 ||
          Walking_y > ScreenHeight) {
        DrawLineEx((Vector2){StartX, StartY}, (Vector2){Walking_x, Walking_y}, thickness, WHITE); 
        break;
      }
      if (sqrtf((Walking_x - light->x)*(Walking_x - light->x)+
                (Walking_y - light->y)*(Walking_y - light->y)) > 400) {
        DrawLineEx((Vector2){StartX, StartY}, (Vector2){Walking_x, Walking_y}, thickness, WHITE); 
        break;
      }
      int hit = -1;
      for (int j = 0; j < count; j++) {
        if (PointInsideObject((Vector2){Walking_x, Walking_y}, &obstacles[j])==true) {
          hit = j;
          break;
        }
      }
      if (hit >= 0) {
        const Obstacle *object = &obstacles[hit];
        DrawLineEx((Vector2){StartX, StartY}, (Vector2){Walking_x, Walking_y}, thickness, WHITE); 
        if (object->reflective && bounces < MaxBounces) {
          Vector2 norm = NormVector((Vector2){Walking_x, Walking_y}, object);
          float DotProduct = dx * norm.x + dy * norm.y;
          dx = dx - 2 * DotProduct * norm.x;
          dy = dy - 2 * DotProduct * norm.y;
          Walking_x += dx * step * 2;
          Walking_y += dy * step * 2;
          StartX = Walking_x;
          StartY = Walking_y;
          bounces++;
          continue;
        }
        break;
      }
    }
  }
}
