#include "light_c.h"
#include "obstacle_c.h"
#include <math.h>    // DIFF: <cmath> is C++. In C this is <math.h>
#include <stdbool.h> // DIFF: bool/true/false don't exist in C without this header.
                     // In C++ bool is a built-in type, no include needed.

// DIFF: 'static' here still means file-private (same as C++). This one is the
// same! DIFF: 'const Obstacle &obstacle' (C++ reference) → 'const Obstacle
// *obstacle' (C pointer) References are a C++ feature. In C you always pass a
// pointer explicitly.
static bool PointInsideObject(Vector2 point,
                              const Obstacle *obstacle) { // DIFF: & → *
  if (obstacle->type ==
      KASSI) // DIFF: obstacle. → obstacle->  (pointer dereference)
    return CheckCollisionPointRec(point, obstacle->rect);
  else
    return CheckCollisionPointCircle(point, obstacle->center, obstacle->radius);
}

// DIFF: Same reference → pointer change here
static Vector2 NormVector(Vector2 point,
                          const Obstacle *object) { // DIFF: & → *
  if (object->type == CIRCLE) {                     // DIFF: . → ->
    Vector2 ab = {point.x - object->center.x, point.y - object->center.y};
    float c = sqrtf(ab.x * ab.x + ab.y * ab.y);
    return (Vector2){
        ab.x / c,
        ab.y /
            c}; // DIFF: compound literal needs cast in C: (Vector2){...}
                // In C++ {ab.x/c, ab.y/c} is fine on its own as a return value
  }

  if (object->type == KASSI) {
    float left = point.x - object->rect.x;
    float right = (object->rect.x + object->rect.width) - point.x;
    float top = point.y - object->rect.y;
    float bottom = (object->rect.y + object->rect.height) - point.y;

    float nearest = fminf(fminf(left, right), fminf(top, bottom));
    if (nearest == left)
      return (Vector2){-1, 0}; // DIFF: (Vector2) cast needed in C
    if (nearest == right)
      return (Vector2){1, 0};
    if (nearest == top)
      return (Vector2){
          0,
          -1}; // raylib
               // https://i1.sndcdn.com/artworks-000394759668-wan0rk-t1080x1080.jpg
    else
      return (Vector2){0, 1};
  }
  return (Vector2){0, 0};
}

// DIFF: Constructor Light::Light(int, int) → plain function Light_Create(int,
// int) In C++ the constructor name is tied to the class. In C it's just a
// function name you pick. DIFF: Member initializer list ': x(posX), y(posY)'
// doesn't exist in C. You just assign fields manually.
Light Light_Create(int posX, int posY) {
  Light light;    // DIFF: no constructor call, just declare the struct
  light.x = posX; // DIFF: manual field assignment instead of initializer list
  light.y = posY;
  light.RayCount = 3600;
  return light;
}

// DIFF: 'void Light::Draw(...)' → 'void Light_Draw(const Light* light, ...)'
// The Light:: scope prefix disappears. Instead we take an explicit pointer to
// the Light struct (the thing C++ calls 'this' implicitly).
void Light_Draw(const Light *light, const Obstacle *obstacles,
                int count) { // DIFF: explicit Light* param
  int ScreenWidth = GetScreenWidth();
  int ScreenHeight = GetScreenHeight();
  float step = 2.0f;
  int MaxBounces = 5;

  for (int i = 0; i < light->RayCount;
       i++) { // DIFF: RayCount → light->RayCount (explicit this)
    float angle = (i * 360.0f / light->RayCount) * DEG2RAD;
    float dx = cosf(angle);
    float dy = sinf(angle);

    float Walking_x = (float)light->x; // DIFF: x → light->x
    float Walking_y = (float)light->y; // DIFF: y → light->y
    float StartX = Walking_x;
    float StartY = Walking_y;
    int bounces = 0;

    while (1) { // DIFF: 'while (true)' → 'while (1)'. true needs stdbool.h in
                // C, using 1 directly is more idiomatic C.
      Walking_x += dx * step;
      Walking_y += dy * step;

      if (Walking_x < 0 || Walking_x > ScreenWidth || Walking_y < 0 ||
          Walking_y > ScreenHeight) {
        DrawLine(StartX, StartY, Walking_x, Walking_y, Fade(YELLOW, 0.3f));
        break;
      }
      if (sqrtf((Walking_x - light->x) *
                    (Walking_x - light->x) + // DIFF: x → light->x
                (Walking_y - light->y) * (Walking_y - light->y)) > 600) {
        DrawLine(StartX, StartY, Walking_x, Walking_y, Fade(YELLOW, 0.3f));
        break;
      }

      int hit = -1;
      for (int j = 0; j < count; j++) {
        // DIFF: obstacles[j] is passed as pointer with & because our function
        // takes const Obstacle* In C++ we passed a const reference, which binds
        // directly to obstacles[j]. In C we take the address explicitly.
        if (PointInsideObject((Vector2){Walking_x, Walking_y}, &obstacles[j]) ==
            true) { // DIFF: & needed, (Vector2) cast
          hit = j;
          break;
        }
      }

      if (hit >= 0) {
        const Obstacle *object =
            &obstacles[hit]; // DIFF: 'const Obstacle &' → 'const Obstacle *'
                             // and we take the address with &
        DrawLine(StartX, StartY, Walking_x, Walking_y, Fade(YELLOW, 0.3f));

        if (object->reflective &&
            bounces < MaxBounces) { // DIFF: object. → object->
          // DIFF: NormVector now takes a pointer, so pass address of the inline
          // struct. In C++ you'd pass a temporary Vector2 by reference; in C
          // you must make it a compound literal and pass it by value (the
          // function takes Vector2 by value anyway).
          Vector2 norm =
              NormVector((Vector2){Walking_x, Walking_y},
                         object); // DIFF: & for object, (Vector2) cast
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
