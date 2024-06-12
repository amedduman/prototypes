#include "defer.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define POINTS_NUM 2
#define STICKS_NUM 1
#define BOUNCE 0.9f
#define GRAVITY 0.5f
#define FRICTION 0.99f

typedef struct
{
  Vector2 pos;
  Vector2 oldPos;
} point;

void point_update(point* point)
{
  Vector2 vel = Vector2Subtract(point->pos, point->oldPos);
  vel = ut_Vector2MulVal(vel, FRICTION);
  point->oldPos = point->pos;
  point->pos = Vector2Add(point->pos, vel);
  point->pos.y += GRAVITY;

  // check for screen bounds
  int w = GetScreenWidth();
  int h = GetScreenHeight();
  if (point->pos.x > w || point->pos.x < 0)
  {
    point->pos.x = point->pos.x > 0 ? w : 0;
    point->oldPos.x = point->pos.x + vel.x * BOUNCE;
  }
  if(point->pos.y > h || point->pos.y < 0)
  {
    point->pos.y = point->pos.y > 0 ? h : 0;
    point->oldPos.y = point->pos.y + vel.y * BOUNCE;
  }
}

void point_draw(point* point)
{
  DrawCircleV(point->pos, 5, WHITE);
}

typedef struct
{
  point* p0;
  point* p1;
  float length;
} stick;

stick stick_create(point* p0, point* p1)
{
  return (stick){p0, p1, Vector2Distance(p0->pos, p1->pos)};
}

void stick_update(stick* stick)
{
  float dist = Vector2Distance(stick->p1->pos, stick->p0->pos);
  float diff = stick->length - dist;
  float percent = diff / dist * 0.5f;
  
  stick->p0->pos = Vector2AddValue(stick->p0->pos, dist * percent * -1.0f);
  stick->p1->pos = Vector2AddValue(stick->p1->pos, dist * percent);
}

void stick_draw(stick* stick)
{
  DrawLineEx(stick->p0->pos, stick->p1->pos, 3, WHITE);
}

int main() 
{
  const int screenWidth = 400;
  const int screenHeight = 400;

  defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow()) 
  {
    SetTargetFPS(60);

    point points[POINTS_NUM];
    points[0] = (point){{100,100}, {95,95}};
    points[1] = (point){{200, 100}, {200, 100}};

    stick sticks[1];

    sticks[0] = stick_create(&points[0], &points[1]);

    while (!WindowShouldClose()) 
    {

      for (int i = 0; i < POINTS_NUM; i++)
      {
        point_update(&points[i]);
      }

      for (int i = 0; i < STICKS_NUM; i++)
      {
        stick_update(&sticks[i]);
      }

      defer(BeginDrawing(), EndDrawing()) 
      {
        ClearBackground(GOLD);
        for (int i = 0; i < STICKS_NUM; i++)
        {
          stick_draw(&sticks[i]);
        }
        for (int i = 0; i < POINTS_NUM; i++)
        {
          point_draw(&points[i]);
        }

      }
    }
  }
}