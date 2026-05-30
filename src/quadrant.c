#include "quadrant.h"
#include <math.h>
#include <raylib.h>

#define QUADRANT_N 12
#define LIGHT_RADIUS 0.10
#define TOTAL_RADIUS 500

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024

float lightRadius = TOTAL_RADIUS * LIGHT_RADIUS;
float quadrantPx = 360.0 / QUADRANT_N;
Vector2 centerV = {SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5};

void DrawLight() { DrawCircleV(centerV, lightRadius, GOLD); }
void DrawHoroscopeMap() { DrawCircleV(centerV, TOTAL_RADIUS, BEIGE); }
void DrawQuadrantLines() {
  for (int i = 0; i < QUADRANT_N / 2; i++) {
    float rad = DEG2RAD * (i * quadrantPx);
    Vector2 startV = {centerV.x + cosf(rad) * TOTAL_RADIUS,
                      centerV.y + sinf(rad) * TOTAL_RADIUS};
    Vector2 endV = {centerV.x + cosf(PI + rad) * TOTAL_RADIUS,
                    centerV.y + sinf(PI + rad) * TOTAL_RADIUS};

    DrawLineV(startV, endV, DARKGRAY);
  }
}

int main(void) {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "planets stuff");

  while (!WindowShouldClose()) {

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawHoroscopeMap();
    DrawQuadrantLines();
    DrawLight();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
