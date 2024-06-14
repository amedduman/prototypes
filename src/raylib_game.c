#include "raylib.h"
#include "utils.h"
#include "segment.h"

int main() 
{
  const int screenWidth = 400;
  const int screenHeight = 400;

  InitWindow(screenWidth, screenHeight, "Math");
  SetTargetFPS(60);

  segment seg = segment_create((Vector2){100,100}, 50, -PI / 4);

  while (!WindowShouldClose()) 
  {


    BeginDrawing();
    ClearBackground(GOLD);

    segment_draw(&seg);

    EndDrawing();
  }
  CloseWindow();
}