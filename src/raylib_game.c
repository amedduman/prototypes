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

void drawLine(Vector2 p0, Vector2 p1) {
  DrawLine(p0.x, p0.y, p1.x, p1.y, BLACK);
}

void translateModel(point *points, int arrLen, Vector3 translateVec) {
  for (int i = 0; i < arrLen; i++) {
    points[i].pos = Vector3Add(points[i].pos, translateVec);
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

  defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow()) {
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

      Vector3 translateVec = {0, 0, 0};

      if (IsKeyDown(KEY_A))
        translateVec.x = -1;
      else if (IsKeyDown(KEY_D))
        translateVec.x = 1;

      if (IsKeyDown(KEY_W))
        translateVec.y = -1;
      else if (IsKeyDown(KEY_S))
        translateVec.y = 1;

      if (IsKeyDown(KEY_UP))
        translateVec.z = 1;
      else if (IsKeyDown(KEY_DOWN))
        translateVec.z = -1;

      translateModel(points, arrLen, utVecMulVal(translateVec, 10));

      project(points, arrLen, fl, screenWidth, screenHeight);

      defer(BeginDrawing(), EndDrawing()) { ClearBackground(GOLD); }

      drawLine(points[0].screen_pos, points[1].screen_pos);
      drawLine(points[1].screen_pos, points[2].screen_pos);
      drawLine(points[2].screen_pos, points[3].screen_pos);
      drawLine(points[3].screen_pos, points[0].screen_pos);
      drawLine(points[4].screen_pos, points[5].screen_pos);
      drawLine(points[5].screen_pos, points[6].screen_pos);
      drawLine(points[6].screen_pos, points[7].screen_pos);
      drawLine(points[7].screen_pos, points[4].screen_pos);
      drawLine(points[0].screen_pos, points[4].screen_pos);
      drawLine(points[1].screen_pos, points[5].screen_pos);
      drawLine(points[2].screen_pos, points[6].screen_pos);
      drawLine(points[3].screen_pos, points[7].screen_pos);
    }
  }
}