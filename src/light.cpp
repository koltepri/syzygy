#include "light.h"
#include "obstacle.h"
#include <cmath> //math er í arc, meðan reylib er í deg, kms

// functions
static bool PointInsideObject(Vector2 point,
                              const Obstacle &obstacle) { // static = private
  if (obstacle.type == KASSI)
    return CheckCollisionPointRec(point, obstacle.rect);
  else
    return CheckCollisionPointCircle(point, obstacle.center, obstacle.radius);
}

static Vector2 NormVector(Vector2 point, const Obstacle &object) {
  if (object.type == CIRCLE) {
    Vector2 ab = {point.x - object.center.x, point.y - object.center.y};
    float c = sqrtf(ab.x * ab.x + ab.y * ab.y);
    return {ab.x / c, ab.y / c};
  } // normalized dx dy stefnur

  if (object.type == KASSI) {
    float left = point.x - object.rect.x;
    float right = (object.rect.x + object.rect.width) - point.x;
    float top = point.y - object.rect.y;
    float bottom = (object.rect.y + object.rect.height) - point.y;

    float nearest = fminf(fminf(left, right), fminf(top, bottom));
    if (nearest == left)
      return {-1, 0};
    if (nearest == right)
      return {1, 0};
    if (nearest == top)
      return {
          0,
          -1}; // raylib
               // https://i1.sndcdn.com/artworks-000394759668-wan0rk-t1080x1080.jpg
    else
      return {0, 1};
  }
  return {0, 0};
}

Light::Light(int posX, int posY) : x(posX), y(posY), RayCount(3600) {}

void Light::Draw(const Obstacle *obstacles, int count) const {
  int ScreenWidth = GetScreenWidth();
  int ScreenHeight = GetScreenHeight();
  float step = 2.0f; // 2 pixlar per skref
  int MaxBounces = 5;

  for (int i = 0; i < RayCount; i++) {
    float angle = (i * 360.0f / RayCount) * DEG2RAD;
    float dx = cosf(angle);
    float dy = sinf(angle);

    float Walking_x = (float)x;
    float Walking_y = (float)y;
    float StartX = Walking_x;
    float StartY = Walking_y;
    int bounces = 0;

    while (true) {
      Walking_x += dx * step;
      Walking_y += dy * step;
      // hætta að teikna condtion
      if (Walking_x < 0 || Walking_x > ScreenWidth || Walking_y < 0 ||
          Walking_y > ScreenHeight) {
        DrawLine(StartX, StartY, Walking_x, Walking_y, Fade(YELLOW, 0.3f));
        break;
      }
      if (sqrtf((Walking_x - x) * (Walking_x - x) +
                (Walking_y - y) * (Walking_y - y)) > 600) {
        DrawLine(StartX, StartY, Walking_x, Walking_y, Fade(YELLOW, 0.3f));
        break;
      }

      // bruh this is hard :(((
      int hit = -1;
      for (int j = 0; j < count; j++) {
        if (PointInsideObject({Walking_x, Walking_y}, obstacles[j]) == true) {
          hit = j; // muna hvaða obstacle var klest á
          break;
        }
      }

      if (hit >= 0) {
        const Obstacle &object = obstacles[hit];
        DrawLine(StartX, StartY, Walking_x, Walking_y, Fade(YELLOW, 0.3f));

        if (object.reflective && bounces < MaxBounces) {
          Vector2 norm = NormVector({Walking_x, Walking_y}, object);
          float DotProduct = dx * norm.x + dy * norm.y;
          dx = dx - 2 * DotProduct * norm.x;
          dy = dy - 2 * DotProduct * norm.y; // flip

          Walking_x += dx * step * 2; // insta skref áfram pre redefine
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
