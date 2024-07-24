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
    bool is_intersecting;
    Vector3 hit_point;
  } ray_sphere_intersection_info;

  // https://www.youtube.com/watch?v=OCZTVpfMSys
  ray_sphere_intersection_info ray_sphere_intersection(Vector3 so, float sr, Vector3 ro, Vector3 rd)
  {
    Vector3 sphere_to_ray_origin = Vector3Subtract(ro, so);

    float a = Vector3DotProduct(rd, rd);
    float b = 2 * Vector3DotProduct(sphere_to_ray_origin, rd);
    float c = Vector3DotProduct(sphere_to_ray_origin, sphere_to_ray_origin) - (sr * sr);

    float discriminant = (b * b) - (4 * a * c); 

    if (discriminant < 0)
    {
      return (ray_sphere_intersection_info) {false, (Vector3){-1,-1,-1}};
    }

    float t1 = (-b + sqrt(discriminant)) / (2*a);
    float t2 = (-b - sqrt(discriminant)) / (2*a);

    if (t1 > 1)
    {
      Vector3 p = Vector3Add(ro, Vector3Scale(rd, t1));

      return (ray_sphere_intersection_info) {true, p};
    }
    if (t2 > 1)
    {
      Vector3 p = Vector3Add(ro, Vector3Scale(rd, t2));

      return (ray_sphere_intersection_info) {true, p};
    }

    return (ray_sphere_intersection_info) {false, (Vector3){-1,-1,-1}};
  }
// #endregion

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "Game");
    
    Vector2 startPos = {220,220};
    Vector2 endPos = {0,0};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
      endPos = GetMousePosition();

      BeginDrawing();

      ClearBackground(RAYWHITE);

      Vector3 dir = Vector3Normalize(Vector3Subtract((Vector3){endPos.x, endPos.y, 0}, (Vector3){startPos.x, startPos.y, 0}));
      dir = Vector3Subtract((Vector3){endPos.x, endPos.y, 0}, (Vector3){startPos.x, startPos.y, 0});

      ray_sphere_intersection_info intersection_info = ray_sphere_intersection(
      (Vector3){200,200,0},
      50, 
      (Vector3){startPos.x,startPos.y,0},
      dir 
      );

      if (intersection_info.is_intersecting)
      {
        DrawCircleV((Vector2){200,200}, 50, GREEN);
        DrawCircleV((Vector2){intersection_info.hit_point.x, intersection_info.hit_point.y}, 10, BLACK);
      }
      else
      {
        DrawCircleV((Vector2){200,200}, 50, RED);
      }

      DrawLineEx(startPos, endPos, 3, BLACK);
      
      EndDrawing();
    }
    CloseWindow();
    return 0;
}