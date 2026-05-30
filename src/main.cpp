#include <raylib.h>
#include "light.h"
int main()
{   int screenWidth = 1500; // finna leið til að kíkja á skjainn og stilla sjálfkrafa  GetScreenWidth() virkar ekki
    int screenHeight = 1000;
    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(60);


    Light light1(screenWidth/2, screenHeight/2);
    Light light2(1000, 1000);
    Obstacle obstacles[] = {
        //{ KASSI,  {800,200,40,250},     {0,0},      0,     false,    DARKGRAY },
        //{ KASSI,  {400,600,300,20},     {0,0},      0,     false,     DARKGRAY  },
        { CIRCLE, {0,0,0,0},            {200,800},  40,    true,    GRAY, 0  },
        { CIRCLE, {0,0,0,0},            {400,600},  60,    false,   MAROON, 0      },
        { CIRCLE, {0,0,0,0},            {600,400},  70,    false,    BEIGE, 0      },
        { CIRCLE, {0,0,0,0},            {800,200},  80,    true,    YELLOW, 0     },};
    int count = sizeof(obstacles)/sizeof(obstacles[0]);    
    int DragIndex = -1; //-1 = free solo


    Texture2D Mars = LoadTexture("Images/Mars.png");

    while (!WindowShouldClose()){
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < count; i++) {
                if (obstacles[i].type == CIRCLE &&
                    CheckCollisionPointCircle(mouse, obstacles[i].center, obstacles[i].radius)) {
                    DragIndex = i;
                    break;}}}
        
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            DragIndex = -1;}

        if (DragIndex >= 0) {
            obstacles[DragIndex].center = GetMousePosition();}
    
        BeginDrawing();
            ClearBackground(BLACK);
            light1.Draw(obstacles, count);
            light2.Draw(obstacles, count);
            for (int i = 0; i < count; i++) {
                if (obstacles[i].type == KASSI)
                    DrawRectangleRec(obstacles[i].rect, obstacles[i].color);
                else if (obstacles[i].type== CIRCLE) {
                    if (i==2)
                        DrawTexturePro(Mars, {0,0,(float) Mars.width,(float) Mars.height}, {obstacles[i].center.x-obstacles[i].radius,obstacles[i].center.y-obstacles[i].radius, obstacles[i].radius*2, obstacles[i].radius*2}, {0,0}, 0, WHITE);
                    else
                        DrawCircleV(obstacles[i].center, obstacles[i].radius, obstacles[i].color); 
                }
            }
        EndDrawing();
    }
    CloseWindow();
}