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
  struct segment seg3 = segment_create(seg2.end, 50, -PI / 4, MAROON, &seg2);


  float angle = 0;

  while (!WindowShouldClose()) 
  {

    float seg1angle = sinf(angle) * 0.12f;
		float seg2angle = cosf(angle * 0.5f) * 0.092f;
		float seg3angle = sinf(angle * 1.5f) * 0.134f;
    angle += 0.05;

    segment_rotate(&seg1, seg1angle);
    segment_update(&seg1);

    segment_rotate(&seg2, seg2angle);
    segment_update(&seg2);

    segment_rotate(&seg3, seg3angle);
    segment_update(&seg3);

    /////////////////////////////
    BeginDrawing();
    ClearBackground(GOLD);

    segment_draw(&seg1);
    segment_draw(&seg2);
    segment_draw(&seg3);

    EndDrawing();
    /////////////////////////////
  }
  CloseWindow();
}