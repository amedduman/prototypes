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
  
  struct segment seg1 = segment_create(screenCenter, 50, -PI / 4, WHITE, NULL);
  struct segment seg2 = segment_create(seg1.end, 50, -PI / 4, WHITE, &seg1);
  struct segment seg3 = segment_create(seg2.end, 50, -PI / 4, WHITE, &seg2);

  #define POINTS_COUNT 1000
  Vector2 points[POINTS_COUNT];
  int pointsIndex = 0;

  float angle = 0;

  while (!WindowShouldClose()) 
  {

    float seg1angle = sinf(angle) * 0.12f;
		float seg2angle = cosf(angle * 0.5f) * 0.082f;
		float seg3angle = sinf(angle * 1.5f) * 0.134f;
    angle += 0.03;

    segment_rotate(&seg1, seg1angle);
    segment_update(&seg1);

    segment_rotate(&seg2, seg2angle);
    segment_update(&seg2);

    segment_rotate(&seg3, seg3angle);
    segment_update(&seg3);

    
    if (pointsIndex < POINTS_COUNT)
    {
      points[pointsIndex] = seg3.end;
    }
    pointsIndex++;

    /////////////////////////////
    BeginDrawing();
    ClearBackground(GOLD);

    DrawLineStrip(points, pointsIndex, BLACK);

    segment_draw(&seg1);
    segment_draw(&seg2);
    segment_draw(&seg3);

    EndDrawing();
    /////////////////////////////
  }
  CloseWindow();
}