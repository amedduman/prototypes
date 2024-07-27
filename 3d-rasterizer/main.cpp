#include "include/raylib.h"
#include "include/raymath.h"
#include <iostream>
#include <assert.h>

#pragma region print

template <typename T>
void print(const T& msg)
{
  using namespace std;
  cout << msg << endl;
}

#pragma endregion

#pragma region vector operator overloading

// Operator overloading for Vector2 addition
Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2Add(v1, v2);
}

// Operator overloading for Vector2 output
std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

#pragma endregion

#pragma region canvas

void canvas_put_pixel(int Cx, int Cy, Color color)
{
  int Sx = GetScreenWidth() / 2 + Cx;
  int Sy = GetScreenHeight() / 2 - Cy;

  DrawPixel(Sx, Sy, color);
}


Vector3 canvas_to_viewport(int Cx, int Cy)
{
  float Vw = 1.0f; // the viewport height
  float Vh = 1.0f; // the viewport width
  float d = 1.0f; // the distance from the camera to the canvas

  float Vx = Cx * (Vw / GetScreenWidth());
  float Vy = Cy * (Vh / GetScreenHeight());
  float Vz = d;

  return (Vector3){ Vx, Vy, Vz };
}

#pragma endregion

#pragma region draw line

/*
DrawLine(P0, P1, color)
{
    a = (y1 - y0) / (x1 - x0)
    b = y0 - a * x0
    for x = x0 to x1
    {
        y = a * x + b
        canvas.PutPixel(x, y, color)
    }
}
*/
void line_draw(Vector2 p0, Vector2 p1, Color color)
{
  assert(p1.x > p0.x);

  float a = (float)(p1.y - p0.y) / (float)(p1.x - p0.x);
  float b = p0.y - a * p0.x;

  for (int x = p0.x; x <= p1.x; x++)
  {
    float y = a * x + b;
    canvas_put_pixel(x, y, color);
  }
  
}

#pragma endregion

#pragma region main

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "Game");

    Vector2 x = {0,0};
    Vector2 y = {50,50};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        line_draw(x, y, BLACK);
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

#pragma endregion