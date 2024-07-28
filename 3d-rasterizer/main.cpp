#include "include/raylib.h"
#include "include/raymath.h"
#include <iostream>
#include <assert.h>
#include <vector>

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

void line_draw(Vector2 p0, Vector2 p1, Color color)
{
  float dx = p1.x - p0.x;
  float dy = p1.y - p0.y;

  if (fabsf(dx) > fabsf(dy)) // Line is horizontal-ish
  {
    // we are gonna draw line starting from smaller x value 
    if (p1.x < p0.x)
    {
      std::swap(p0, p1);
    }

    float a = dy / dx; // slope
    float y = p0.y;

    for (int x = p0.x; x <= p1.x; x++)
    {
      canvas_put_pixel(x, y, color);
      y = y + a;
    
    }
  }
  else // Line is vertical-ish
  {
    // we are gonna draw line starting from smaller y value 
    if (p1.y < p0.y)
    {
      std::swap(p0, p1);
    }

    float a = dx / dy; // slope
    float x = p0.x;

    for (int y = p0.y; y <= p1.y; y++)
    {
      canvas_put_pixel(x, y, color);
      x = x + a;
    }
  } 
}

#pragma endregion

#pragma region draw triangle

void triangle_wireframe_draw(Vector2 p0, Vector2 p1, Vector2 p2, Color color)
{
  line_draw(p0, p1, color);
  line_draw(p1, p2, color);
  line_draw(p2, p0, color);
}

typedef struct 
{
  int x;
  int y;
} vec2i_t;

vec2i_t to_vec2i(Vector2 v)
{
  int x = (int)(v.x);
  int y = (int)(v.y);

  vec2i_t result = {};
  result.x = x;
  result.y = y;

  return result;
}

int edge_cross(vec2i_t a, vec2i_t b, vec2i_t p)
{
  vec2i_t ab = {b.x - a.x, b.y - a.y};
  vec2i_t ap = {p.x - a.x, p.y - a.y};

  return ab.x * ap.y - ab.y * ap.x; 
}

int edge_cross_debug(vec2i_t a, vec2i_t b, vec2i_t p)
{
  vec2i_t ab = {b.x - a.x, b.y - a.y};
  vec2i_t ap = {p.x - a.x, p.y - a.y};

  if (p.x == - 20 && p.y == 20)
  {
    print("edge cross");
    print(ab.x);
    print(ab.y);
    print(ap.x);
    print(ap.y);
  }
  return ab.x * ap.y - ab.y * ap.x; 
}

void triangle_draw(Vector2 vertex0, Vector2 vertex1, Vector2 vertex2, Color color)
{
  vec2i_t v0 = to_vec2i(vertex0);
  vec2i_t v1 = to_vec2i(vertex1);
  vec2i_t v2 = to_vec2i(vertex2);

  int x_min = std::min({v0.x, v1.x, v2.x});
  int y_min = std::min({v0.y, v1.y, v2.y});
  int x_max = std::max({v0.x, v1.x, v2.x});
  int y_max = std::max({v0.y, v1.y, v2.y});

  for (int x = x_min; x <= x_max; x++)
  {
    for (int y = y_min; y <= y_max; y++)
    {
      vec2i_t p = {x, y};

      int w0 = edge_cross(v1, v2, p);
      int w1 = edge_cross(v2, v0, p);
      int w2 = edge_cross(v0, v1, p);

      bool is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

      if (is_inside)
      {
        canvas_put_pixel(x, y, color);
      }

      if (x == -20 && y == 20)
      {
        print("put pixel");
        int w0 = edge_cross_debug(v1, v2, p);
        print(w0);
        print("--------");
        canvas_put_pixel(x, y, color);
      }
      
    }
  }
}

#pragma endregion

#pragma region interpolation

std::vector<float> interpolate(int i0, int d0, int i1, int d1)
{
  return interpolate(static_cast<float>(i0), static_cast<float>(d0), static_cast<float>(i1), static_cast<float>(d1));
}

std::vector<float> interpolate(float i0, float d0, float i1, float d1)
{
  assert(i0 < i1);

  std::vector<float> my_values{};

  if (fabsf(i0 - i1) < EPSILON)
  {
    assert(my_values.empty());
    my_values.push_back(d0);
    return my_values;
  }

  float a = (d1 - d0) / (i1 - i0);
  float d = d0;

  for (int i = static_cast<int>(i0); i < static_cast<int>(i1); i++)
  {
    my_values.push_back(d);
    d = d + a;
  }
  
  return my_values;
}

#pragma endregion

#pragma region main

int main(void)
{
  const int screenWidth = 128;
  const int screenHeight = 128;

  InitWindow(screenWidth, screenHeight, "Game");

  Vector2 vertices[4] = {
    {-24, 24},
    {16, 24},
    {-24, -16},
    {26, -26}
  };

  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
      BeginDrawing();

      triangle_wireframe_draw(vertices[0], vertices[1], vertices[2], RED);
      triangle_draw(vertices[0], vertices[1], vertices[2], BLACK);

      ClearBackground(RAYWHITE);
      EndDrawing();
  }
  CloseWindow();
  return 0;
}

#pragma endregion