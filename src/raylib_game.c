#include "raylib.h"
#include "utils.h"
#include "segment.h"
#include <sys/_types/_null.h>

int main() 
{
  const int screenWidth = 400;
  const int screenHeight = 400;

  InitWindow(screenWidth, screenHeight, "Math");
  SetTargetFPS(60);
  Vector2 screenCenter = (Vector2){(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
  
  struct segment seg1 = segment_create(screenCenter, 50, -PI / 4, BLACK, NULL);
  struct segment seg2 = segment_create(seg1.end, 50, -PI / 4, WHITE, &seg1);

  while (!WindowShouldClose()) 
  {

    segment_rotate(&seg1);

    segment_update(&seg1);
    segment_update(&seg2);

    BeginDrawing();
    ClearBackground(GOLD);

    segment_draw(&seg1);
    segment_draw(&seg2);

    EndDrawing();
  }
  CloseWindow();
}