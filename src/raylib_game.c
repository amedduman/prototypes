#include "defer.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

int main() {
  const int screenWidth = 400;
  const int screenHeight = 400;

  defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow()) {

    SetTargetFPS(60);

    float easing = 0.2f;
    int pointsNum = 40;
    Vector2 points[pointsNum];
    for (int i = 0; i < pointsNum; i++) {
      points[i] = (Vector2){0, 0};
    }
    Vector2 target = GetMousePosition();

    while (!WindowShouldClose()) {

      target = GetMousePosition();

      for (int i = 0; i < pointsNum; i++) {

        Vector2 dir = Vector2Subtract(target, points[i]);
        dir = ut_Vector2MulVal(dir, easing);
        points[i] = Vector2Add(dir, points[i]);

        target = points[i];
      }

      defer(BeginDrawing(), EndDrawing()) {
        ClearBackground(GOLD);

        /**/
        for (int i = 0; i < pointsNum - 1; i++) {
          // DrawCircleV(points[i], 8, MAROON);
          DrawLineEx(points[i], points[i + 1], 3, WHITE);
        }
        // DrawLineStrip(points, pointsNum, MAROON);
      }
    }
  }
}