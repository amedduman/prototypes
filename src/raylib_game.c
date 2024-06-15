#include "raylib.h"
#include "utils.h"
#include "segment.h"

int main() 
{
  const int screenWidth = 400;
  const int screenHeight = 400;

  defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow())
  {
    SetTargetFPS(60);
    Vector2 screenCenter = (Vector2){(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};

    float segLength = 10;
    const int seg_count = 20;
    struct segment segs[seg_count];
    
    for (int i = 0; i < seg_count; i++)
    {
      Color col = i == (seg_count - 1) ? BLACK : WHITE;
      segs[i] = segment_create(screenCenter, segLength, 0, col);
    }

    Vector2 fixedPoint = utGetScreenCenter();


    while (!WindowShouldClose()) 
    {
      segs[0].start = fixedPoint;

      segment_follow(&segs[seg_count - 1], GetMousePosition());
      for (int i = seg_count - 2; i >= 0 - 1; i--)
      {
        segment_follow(&segs[i], segs[i + 1].start);
      }

      segs[0].start =  fixedPoint;
      segs[0].end = segment_calc_endS(&segs[0]);
      for (int i = 1; i < seg_count; i++)
      {
        segs[i].start = segs[i - 1].end;
        segs[i].end = segment_calc_endS(&segs[i]);
      }

      defer(BeginDrawing(), EndDrawing())
      {
        ClearBackground(GOLD);

        for (int i = 0; i < seg_count; i++)
        {
          segment_draw(&segs[i]);
        }
      }
    }
  }
}