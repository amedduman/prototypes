#include "include/raylib.h"
#include "include/raymath.h"
#include <assert.h>
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
    Vector3 closest_intersection_point;
    Vector3 farthest_intersection_point;
  } ray_sphere_intersection_info;

  // https://www.youtube.com/watch?v=OCZTVpfMSys
  ray_sphere_intersection_info ray_sphere_intersection(Vector3 so, float sr, Vector3 ro, Vector3 rd)
  {
    assert(Vector3Length(rd) < 1 + EPSILON);

    ray_sphere_intersection_info result = {false, {0,0,0}, {0,0,0}};

    // vector between ray origin and sphere origin 
    Vector3 l = Vector3Subtract(so, ro);
    // dot product of l and rd
    float t = Vector3DotProduct(l, rd);

    // nearest distance between sphere origin and ray 
    float d = sqrtf(Vector3LengthSqr(l) - t*t);

    if (d < 0 || d > sr)
    {
      return (ray_sphere_intersection_info) {false, {-1,-1,-1}, {-1,-1,-1}};
    }

    if (t < 0)
    {
      return (ray_sphere_intersection_info) {false, {-1,-1,-1}, {-1,-1,-1}};
    }

    float x = sqrtf(sr*sr - d*d);
    Vector3 t0 = Vector3Add(ro, Vector3Scale(rd, t - x));
    Vector3 t1 = Vector3Add(ro, Vector3Scale(rd, t + x));
    
    return (ray_sphere_intersection_info) {true, t0, t1};
  }
// #endregion

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "Game");
    
    Vector2 startPos = {200,200};
    Vector2 endPos = {0,0};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
      endPos = GetMousePosition();

      BeginDrawing();

      ClearBackground(RAYWHITE);

      DrawLineV(startPos, endPos, BLACK);

      ray_sphere_intersection_info intersection_info = ray_sphere_intersection(
      (Vector3){200,200,0},
      50, 
      (Vector3){startPos.x,startPos.y,0},
      Vector3Normalize(Vector3Subtract((Vector3){endPos.x, endPos.y, 0}, (Vector3){startPos.x, startPos.y, 0}))
      );

      if (intersection_info.is_intersection_found)
      {
        DrawCircleV((Vector2){200,200}, 50, GREEN);
        DrawCircleV((Vector2){intersection_info.closest_intersection_point.x, intersection_info.closest_intersection_point.y}, 10, BLACK);
      }
      else
      {
        DrawCircleV((Vector2){200,200}, 50, RED);
      }
      
      EndDrawing();
    }
    CloseWindow();
    return 0;
}