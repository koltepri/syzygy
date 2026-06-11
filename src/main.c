#include "light.h"
#include <raylib.h>
#include <stdbool.h> 


int main(void) { 
  int screenWidth = 1500; //todo að það autofillist og allt er ratioað út frá því
  int screenHeight = 1000;
  InitWindow(screenWidth, screenHeight, "");
  SetTargetFPS(60);

  //Load texture fyrir hluti, breyta í function?
  Texture2D background = LoadTexture("Images/Background_Board_rough.png");     
  RenderTexture2D Shadow_Negatative = LoadRenderTexture(screenWidth, screenHeight);  //Shadow_Negative er skugginn sem er síðan skorinn úr af ljósinu, get ekki fundið meira cool og discriptive nafn
  Texture2D Mars = LoadTexture("Images/Mars.png");

  Light light1 = Light_Create(screenWidth/2-210,screenHeight/2); //todo búa til betri leið til að finna miðju á ljósi
  Obstacle obstacles[] = {
      {CIRCLE, {0, 0, 0, 0}, {200, 800}, 40, false, GRAY, 0},
      {CIRCLE, {0, 0, 0, 0}, {400, 600}, 60, false, MAROON, 0},
      {CIRCLE, {0, 0, 0, 0}, {600, 400}, 70, false, BEIGE, 0},
      {CIRCLE, {0, 0, 0, 0}, {800, 200}, 80, false, YELLOW, 0},
  };
  int count = sizeof(obstacles) / sizeof(obstacles[0]);
  int DragIndex = -1;

  while (!WindowShouldClose()) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 mouse = GetMousePosition();
      for (int i = 0; i < count; i++) {
        if (obstacles[i].type == CIRCLE && CheckCollisionPointCircle(mouse, obstacles[i].center,obstacles[i].radius)) {
          DragIndex = i;
          break;}}}
          
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      DragIndex = -1;}
    if (DragIndex >= 0) {
      obstacles[DragIndex].center = GetMousePosition();}


  //Graphics draw
  BeginTextureMode(Shadow_Negatative);
    ClearBackground((Color){128, 128, 128, 255});  //Minor TODO: Fikkta aðeins í hversu mikill skuggi RGB
    Shadow_Removal (&light1, obstacles, count);
  EndTextureMode();
  
  BeginDrawing(); 
    ClearBackground(BLACK);
    DrawTexturePro(background,
        (Rectangle){0, 0, (float)background.width, (float)background.height},
        (Rectangle){0, 0, (float)screenWidth, (float)screenHeight},          
        (Vector2){0, 0}, 0, WHITE);
    BeginBlendMode(BLEND_MULTIPLIED); //blend mode marfaldar pixla saman RBG 0-1 aka fjarlægir skuggan, í staðinn fyrir að overwrita eins og objectin.             
    DrawTextureRec(Shadow_Negatative.texture,(Rectangle){0, 0, (float)Shadow_Negatative.texture.width,
                    -(float)Shadow_Negatative.texture.height},(Vector2){0, 0}, WHITE);
    EndBlendMode();                                               


    //Light_Draw(&light1, obstacles,count); //Vill halda þessu fyrir debug.
    
    for (int i = 0; i < count; i++) {
      if (obstacles[i].type == KASSI)
        DrawRectangleRec(obstacles[i].rect, obstacles[i].color);
      else if (obstacles[i].type == CIRCLE) {
        if (i == 2)
          DrawTexturePro(Mars,
              (Rectangle){0, 0, (float)Mars.width, (float)Mars.height},
              (Rectangle){obstacles[i].center.x - obstacles[i].radius,
                          obstacles[i].center.y - obstacles[i].radius,
                          obstacles[i].radius * 2, obstacles[i].radius * 2},
              (Vector2){0, 0}, 0, WHITE); //https://i.ytimg.com/vi/FhqJnE8DLzA/sddefault.jpg
        else
          DrawCircleV(obstacles[i].center, obstacles[i].radius,
                      obstacles[i].color);
      }
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
