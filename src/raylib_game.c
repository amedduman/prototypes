#include "defer.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

typedef struct {
  Vector3 pos;
  Vector3 size;
  Vector2 screen_pos;
} point;

void project(point *points, int arrLen, float fl, int screenWidth,
             int screenHeight) {
  for (int i = 0; i < arrLen; i++) {
    float perspective = fl / (fl + points[i].pos.z);

    points[i].screen_pos.x =
        points[i].pos.x * perspective + (float)screenWidth / 2;
    points[i].screen_pos.y =
        points[i].pos.y * perspective + (float)screenHeight / 2;
  }
}

int main() {
  const int screenWidth = 800;
  const int screenHeight = 800;

  float fl = 300;
  int arrLen = 8;
  point points[arrLen];
  float radius = 5;

  points[0].pos = (Vector3){-500, -500, 1000};
  points[1].pos = (Vector3){500, -500, 1000};
  points[2].pos = (Vector3){500, -500, 500};
  points[3].pos = (Vector3){-500, -500, 500};
  points[4].pos = (Vector3){-500, 500, 1000};
  points[5].pos = (Vector3){500, 500, 1000};
  points[6].pos = (Vector3){500, 500, 500};
  points[7].pos = (Vector3){-500, 500, 500};

  project(points, arrLen, fl, screenWidth, screenHeight);

  defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow()) {
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

      defer(BeginDrawing(), EndDrawing()) { ClearBackground(GOLD); }

      for (int i = 0; i < arrLen; i++) {
        DrawCircleV(points[i].screen_pos, radius, BLACK);
      }
    }
  }
}