#include "defer.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

float linearTween(float currentTime, float beginningValue, float changeInValue,
                  float duration) {
  // return changeInValue * currentTime / beginningValue + duration;
  return beginningValue + changeInValue * (currentTime / duration);
}

int main() {
  const int screenWidth = 400;
  const int screenHeight = 400;

  defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow()) {

    SetTargetFPS(60);

    Vector2 circlePos = {0, 0};
    Vector2 startPos = utGetScreenCenter();
    Vector2 targetPos = {100, 100};
    float duration = 1;

    float changeX = targetPos.x - startPos.x;
    float changeY = targetPos.y - startPos.y;

    circlePos = startPos;

    float startTime = GetTime();

    while (!WindowShouldClose()) {

      float currentTime = GetTime() - startTime;
      if (currentTime < duration) {
        circlePos.x = linearTween(currentTime, startPos.x, changeX, duration);
        circlePos.y = linearTween(currentTime, startPos.y, changeY, duration);
      }

      defer(BeginDrawing(), EndDrawing()) {
        ClearBackground(GOLD);

        DrawCircleV(circlePos, 8, WHITE);
      }
    }
  }
}