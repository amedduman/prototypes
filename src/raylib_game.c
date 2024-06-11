#include "defer.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

typedef struct {
  bool isIntersecting;
  Vector2 intersectionPoint;
} utLineIntersectionResult;

bool utfloatComp(float a, float b) { return fabs(a - b) < 1e-6; }

bool utisPointOnSegment(Vector2 p, Vector2 p0, Vector2 p1) {
  return p.y  >= fminf(p0.y, p1.y) && p.y  <= fmaxf(p0.y, p1.y) &&
         p.x >= fminf(p0.x, p1.x) && p.x <= fmaxf(p0.x, p1.x);

  /*

  printf("############\n");
  printf("%f\n", p0.y);
  printf("%f\n", p.y);
  printf("%f\n", p1.y);

  return p.y  >= fminf(p0.y, p1.y) && p.y  <= fmaxf(p0.y, p1.y);

  float e = 0.000001f;
  return fminf(p0.x, p1.x) <= (p.x - e) && (p.x + e) <= fmaxf(p0.x, p1.x) &&
         fminf(p0.y, p1.y) <= (p.y - e) && (p.y + e) <= fmaxf(p0.y, p1.y);
  */
}

utLineIntersectionResult utcheckLineIntersect(Vector2 p0, Vector2 p1,
                                              Vector2 p2, Vector2 p3) {
  float A1 = p1.y - p0.y;
  float B1 = p0.x - p1.x;
  float C1 = A1 * p0.x + B1 * p0.y;

  float A2 = p3.y - p2.y;
  float B2 = p2.x - p3.x;
  float C2 = A2 * p2.x + B2 * p2.y;

  float denominator = A1 * B2 - A2 * B1;

  // check if lines are parallel or colinear
  if (utfloatComp(denominator, 0)) {
    return (utLineIntersectionResult){false, {0, 0}};
  }

  float intersectX = (B2 * C1 - B1 * C2) / denominator;
  float intersectY = (A1 * C2 - A2 * C1) / denominator;
  Vector2 intersection = {intersectX, intersectY};
  
  //return (utLineIntersectionResult){true, intersection};

  if (
      utisPointOnSegment(intersection, p0, p1)
      &&
      utisPointOnSegment(intersection, p2, p3)
    ) 
  {
    return (utLineIntersectionResult){true, intersection};
  }
  /*
  if (utisPointOnSegment(intersection, p0, p1) &&
      utisPointOnSegment(intersection, p2, p3)) {
    return (utLineIntersectionResult){true, intersection};
  }
  */
  return (utLineIntersectionResult){false, {0, 0}};
}

void DragCircle(Vector2 *circleCenter, float radius)
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (ut_IsPointCircleIntersect(GetMousePosition(), *circleCenter, radius + radius))
        {
            *circleCenter = GetMousePosition();
        }
    }
}

int main() {
  const int screenWidth = 400;
  const int screenHeight = 400;

  defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow()) {

    SetTargetFPS(60);

    Vector2 p0 = {100, 100};
    Vector2 p1 = {100, 350};
    Vector2 p2 = {0, 300};
    Vector2 p3 = {350, 300};

    while (!WindowShouldClose()) {

      defer(BeginDrawing(), EndDrawing()) {
        ClearBackground(GOLD);
        //
        //p2 = GetMousePosition();
        DragCircle(&p0, 10);
        DragCircle(&p1, 10);
        DragCircle(&p2, 10);
        DragCircle(&p3, 10);

        utLineIntersectionResult result = utcheckLineIntersect(p0, p1, p2, p3);
        if (result.isIntersecting) {
          DrawCircleV(result.intersectionPoint, 15, MAROON);
        }
        DrawLineEx(p0, p1, 3, WHITE);
        DrawLineEx(p2, p3, 3, BLACK);
      }
    }
  }
}