#include "include/raylib.h"
#include "include/raymath.h"

// #region Canvas

  #define Cw GetScreenWidth() // the canvas width
  #define Ch GetScreenHeight() // the canvas height
  void canvas_put_pixel(int Cx, int Cy, Color color)
  {
    int Sx = Cw / 2 + Cx;
    int Sy = Ch / 2 - Cy;

    DrawPixel(Sx, Sy, color);
  }

  #define Vw 1.0f // the viewport height
  #define Vh 1.0f // the viewport width
  #define d 1.0f // the distance from the camera to the canvas
  Vector3 canvas_to_viewport(int Cx, int Cy, int Cw, int Ch)
  {
    float Vx = Cx * Vw / Cw;
    float Vy = Cy * Vh / Ch;
    float Vz = 0.0f;

    return (Vector3){ Vx, Vy, Vz };
  }

// #endregion

// #region Raycasting
  bool raycasting_sphere_instersection(Vector3 sphere_origin, float sphere_radius, Vector3 ray_origin, Vector3 ray_direction)
  {
    float distance = Vector3DotProduct(Vector3Subtract(sphere_origin, ray_origin), ray_direction);
    Vector3 closest_point_to_sphere = Vector3Add(ray_origin, Vector3Scale(ray_direction, distance));
    float distance_to_sphere = Vector3Distance(closest_point_to_sphere, sphere_origin);
    return distance_to_sphere <= sphere_radius;
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

      is_intersecting = raycasting_sphere_instersection(
       (Vector3){200,200,0},
       50, 
       (Vector3){0,0,0},
       Vector3Normalize(Vector3Subtract((Vector3){endPos.x, endPos.y, 0}, (Vector3){startPos.x, startPos.y, 0}))
       );

      Color circleColor = (is_intersecting) ? RED : GREEN;
      DrawCircleV((Vector2){200,200}, 50, circleColor);
      EndDrawing();
    }
    CloseWindow();
    return 0;
}