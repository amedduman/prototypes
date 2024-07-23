#include "include/raylib.h"
#include "include/raymath.h"
#include <math.h>
#include <stdio.h>

// #region Canvas

  void canvas_put_pixel(int Cx, int Cy, Color color)
  {
    float Cw = GetScreenWidth(); // the canvas width
    float Ch = GetScreenHeight(); // the canvas height

    int Sx = Cw / 2 + Cx;
    int Sy = Ch / 2 - Cy;

    DrawPixel(Sx, Sy, color);
  }

 
  Vector3 canvas_to_viewport(int Cx, int Cy, int Cw, int Ch)
  {
    float Vw = 1.0f; // the viewport height
    float Vh = 1.0f; // the viewport width
    float d = 1.0f; // the distance from the camera to the canvas

    float Vx = Cx * Vw / Cw;
    float Vy = Cy * Vh / Ch;
    float Vz = d;

    return (Vector3){ Vx, Vy, Vz };
  }

// #endregion

// #region Raycasting

  typedef struct
  {
    bool is_intersection_found;
    Vector2 points;
    Vector3 closest_intersection_point;
    Vector3 farthest_intersection_point;
  } ray_sphere_intersection_info;

  // https://www.youtube.com/watch?v=OCZTVpfMSys
  ray_sphere_intersection_info ray_sphere_intersection(Vector3 so, float sr, Vector3 ro, Vector3 rd)
  {
    ray_sphere_intersection_info result = {false, {0,0,0}, {0,0,0}};

    float t = Vector3DotProduct(Vector3Subtract(so, ro), rd);
    Vector3 p = Vector3Add(ro, Vector3Scale(rd, t));
    float y = Vector3Distance(p, so);

    if (y <= sr)
    {
      result.is_intersection_found = true;

      float x = sqrtf(sr * sr - y * y);
      float t1 = fmaxf(t - x, 0);
      float t2 = t + x;
      
      result.points = (Vector2){t1,t2};
      result.closest_intersection_point = Vector3Add(ro, Vector3Scale(rd, t1));
      result.farthest_intersection_point = Vector3Add(ro, Vector3Scale(rd, t2));
    }

    return result;
  }
// #endregion

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "Game");
    
    bool is_intersecting = false;
    Vector2 startPos = {0,0};
    Vector2 endPos = {0,0};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
      endPos = GetMousePosition();

      BeginDrawing();

      ClearBackground(RAYWHITE);

      DrawLineV(startPos, endPos, BLACK);

      is_intersecting = ray_sphere_intersection(
       (Vector3){200,200,0},
       50, 
       (Vector3){0,0,0},
       Vector3Normalize(Vector3Subtract((Vector3){endPos.x, endPos.y, 0}, (Vector3){startPos.x, startPos.y, 0}))
       ).is_intersection_found;

       ray_sphere_intersection_info intersection_info = ray_sphere_intersection(
       (Vector3){200,200,0},
       50, 
       (Vector3){0,0,0},
       Vector3Normalize(Vector3Subtract((Vector3){endPos.x, endPos.y, 0}, (Vector3){startPos.x, startPos.y, 0}))
       );

      Color circleColor = (is_intersecting) ? RED : GREEN;
      DrawCircleV((Vector2){200,200}, 50, circleColor);
      printf("x: %f, y : %f \n", intersection_info.points.x, intersection_info.points.y);
      //DrawCircleV(Vector2Add(intersection_info.points, (Vector2){200,200}), 10, BLACK);
      DrawCircleV((Vector2){intersection_info.closest_intersection_point.x, intersection_info.closest_intersection_point.y}, 10, BLACK);
      EndDrawing();

      
    }
    CloseWindow();
    return 0;
}