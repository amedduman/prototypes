#include "defer.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define POINTS_NUM 1

typedef struct
{
  Vector2 pos;
  Vector2 oldPos;
} point;

void point_update(point* point)
{
  Vector2 vel = Vector2Subtract(point->pos, point->oldPos);
  point->oldPos = point->pos;
  point->pos = Vector2Add(point->pos, vel);

  // check for screen bounds
  int w = GetScreenWidth();
  int h = GetScreenHeight();
  if (point->pos.x > w || point->pos.x < 0)
  {
    point->pos.x = point->pos.x > 0 ? w : 0;
    point->oldPos.x = point->pos.x + vel.x;
  }
  if(point->pos.y > h || point->pos.y < 0)
  {
    point->pos.y = point->pos.y > 0 ? h : 0;
    point->oldPos.y = point->pos.y + vel.y;
  }
}

void point_draw(point* point)
{
  DrawCircleV(point->pos, 5, WHITE);
}

int main() 
{
  const int screenWidth = 400;
  const int screenHeight = 400;

  defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow()) 
  {
    SetTargetFPS(60);

    point points[POINTS_NUM];
    points[0] = (point){{100,100}, {92,95}};

    while (!WindowShouldClose()) 
    {

      for (int i = 0; i < POINTS_NUM; i++)
      {
        point_update(&points[i]);
      }

      defer(BeginDrawing(), EndDrawing()) 
      {
        ClearBackground(GOLD);

        for (int i = 0; i < POINTS_NUM; i++)
        {
          point_draw(&points[i]);
        }
      }
    }
  }
}