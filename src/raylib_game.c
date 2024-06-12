#include "defer.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define POINTS_NUM 4
#define STICKS_NUM 5
#define BOUNCE 0.9f
#define GRAVITY 0.5f
#define FRICTION 0.99f

typedef struct
{
  Vector2 pos;
  Vector2 oldPos;
} point;

Vector2 point_getVelocity(point* point)
{
  Vector2 vel = Vector2Subtract(point->pos, point->oldPos);
  vel = ut_Vector2MulVal(vel, FRICTION);
  return vel;
}

void point_update(point* point)
{
  Vector2 vel = point_getVelocity(point);
  point->oldPos = point->pos;
  point->pos = Vector2Add(point->pos, vel);
  point->pos.y += GRAVITY;
}

void point_constrain(point* point)
{
  Vector2 vel = point_getVelocity(point);

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
  float dx = stick->p1->pos.x - stick->p0->pos.x;
	float dy = stick->p1->pos.y - stick->p0->pos.y;

  float distance = sqrtf(dx * dx + dy * dy);
  float difference = stick->length - distance;
	float percent = difference / distance / 2;

  float offsetX = dx * percent;
  float offsetY = dy * percent;

  stick->p0->pos.x -= offsetX;
  stick->p0->pos.y -= offsetY;
  stick->p1->pos.x += offsetX;
  stick->p1->pos.y += offsetY;
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
    points[0] = (point){{100,100}, {85,95}};
    points[1] = (point){{200, 100}, {200, 100}};
    points[2] = (point){{200,200}, {200,200}};
    points[3] = (point){{100, 200}, {100, 200}};

    stick sticks[STICKS_NUM];
    sticks[0] = stick_create(&points[0], &points[1]);
    sticks[1] = stick_create(&points[1], &points[2]);
    sticks[2] = stick_create(&points[2], &points[3]);
    sticks[3] = stick_create(&points[3], &points[0]);
    sticks[4] = stick_create(&points[0], &points[2]);

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

      for (int i = 0; i < POINTS_NUM; i++)
      {
        point_constrain(&points[i]);
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