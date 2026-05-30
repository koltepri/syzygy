#include "light_c.h"
#include <raylib.h>
#include <stdbool.h> // DIFF: needed in C for 'bool', 'true', 'false'

// DIFF: 'int main()' with no params is valid in both C and C++.
// Strictly speaking, C prefers 'int main(void)' to explicitly mean no
// parameters. 'int main()' in C means "unspecified parameters" (old C style),
// not zero parameters.
int main(void) { // DIFF: () → (void) for correctness in C
  int screenWidth = 1500;
  int screenHeight = 1000;
  InitWindow(screenWidth, screenHeight, "");
  SetTargetFPS(60);

  // DIFF: 'Light light1(screenWidth/2, screenHeight/2)' is a constructor call
  // in C++. In C there are no constructors. We call our Light_Create function
  // instead and store the returned struct in a variable.
  Light light1 = Light_Create(
      screenWidth / 2,
      screenHeight / 2); // DIFF: constructor → function + assignment
  Light light2 = Light_Create(1000, 1000);

  Obstacle obstacles[] = {
      // DIFF: Struct initializers with named fields (.field = value) work in
      // both C99+ and C++11+.
      // But bare positional initializers like {CIRCLE, {0,0,0,0}, {200,800},
      // 40, true, GRAY, 0}
      // are also valid in both. No change needed here.
      {CIRCLE, {0, 0, 0, 0}, {200, 800}, 40, true, GRAY, 0},
      {CIRCLE, {0, 0, 0, 0}, {400, 600}, 60, false, MAROON, 0},
      {CIRCLE, {0, 0, 0, 0}, {600, 400}, 70, false, BEIGE, 0},
      {CIRCLE, {0, 0, 0, 0}, {800, 200}, 80, true, YELLOW, 0},
  };
  int count = sizeof(obstacles) / sizeof(obstacles[0]);
  int DragIndex = -1;

  Texture2D Mars = LoadTexture("Images/Mars.png");

  while (!WindowShouldClose()) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 mouse = GetMousePosition();
      for (int i = 0; i < count; i++) {
        if (obstacles[i].type == CIRCLE &&
            CheckCollisionPointCircle(mouse, obstacles[i].center,
                                      obstacles[i].radius)) {
          DragIndex = i;
          break;
        }
      }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      DragIndex = -1;
    }

    if (DragIndex >= 0) {
      obstacles[DragIndex].center = GetMousePosition();
    }

    BeginDrawing();
    ClearBackground(BLACK);

    // DIFF: light1.Draw(obstacles, count) → Light_Draw(&light1, obstacles,
    // count) C++ calls the method on the object automatically passing 'this'.
    // In C we pass the address of the struct manually as the first argument.
    Light_Draw(&light1, obstacles,
               count); // DIFF: method call → function call with explicit &
    Light_Draw(&light2, obstacles, count);

    for (int i = 0; i < count; i++) {
      if (obstacles[i].type == KASSI)
        DrawRectangleRec(obstacles[i].rect, obstacles[i].color);
      else if (obstacles[i].type == CIRCLE) {
        if (i == 2)
          DrawTexturePro(
              Mars,
              (Rectangle){
                  0, 0, (float)Mars.width,
                  (float)Mars.height}, // DIFF: (Rectangle) cast needed in C
              (Rectangle){obstacles[i].center.x - obstacles[i].radius,
                          obstacles[i].center.y - obstacles[i].radius,
                          obstacles[i].radius * 2, obstacles[i].radius * 2},
              (Vector2){0, 0}, 0, WHITE); // DIFF: (Vector2) cast
        else
          DrawCircleV(obstacles[i].center, obstacles[i].radius,
                      obstacles[i].color);
      }
    }
    EndDrawing();
  }
  CloseWindow();

  // DIFF: In C, main must return an int. C++ allows omitting the return
  // statement from main (implicitly returns 0). In C99+ this is also allowed,
  // but being explicit is better practice.
  return 0; // DIFF: good habit in C
}
