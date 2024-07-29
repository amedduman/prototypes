#pragma region includes

#include "include/raylib.h"
#include "include/raymath.h"
#include <iostream>
#include <assert.h>
#include <vector>

#pragma endregion 

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

#pragma region structs

typedef struct 
{
  int x;
  int y;
} vec2i_t;

typedef struct
{
  Vector3 pos;
  Color color;
} vertex_t;

Vector2 to_vec2(Vector3 v)
{
  return (Vector2){v.x, v.y};
}

#pragma endregion

#pragma region canvas - viewport

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

vec2i_t canvas_to_screen(int Cx, int Cy)
{
  int screen_x = Cx + (GetScreenWidth() / 2);
  int screen_y = (GetScreenWidth() / 2) - Cy;

  return (vec2i_t){screen_x, screen_y};
}

vec2i_t canvas_to_screen(Vector2 v)
{
  return canvas_to_screen(v.x, v.y);
}

Vector2 viewport_to_canvas(float x, float y)
{
  int Cw = GetScreenWidth();
  int Ch = GetScreenHeight();

  float Vw = 1.0f; // the viewport height
  float Vh = 1.0f; // the viewport width
  float d = 1.0f; // the distance from the camera to the canvas

  return (Vector2){x * (Cw / Vw), y * (Ch / Vh)};
}

#pragma endregion

#pragma region projection

Vector2 ProjectVertex(Vector3 v)
{ 
  float d = 1.0f; // the distance from the camera to the canvas
  
  return viewport_to_canvas(v.x * d / v.z, v.y * d / v.z);
}

#pragma endregion

#pragma region draw line

// p0 and p1 expected to be in canvas space
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

// finds the cross product beween ab and ap vectors
int edge_cross(vec2i_t a, vec2i_t b, vec2i_t p)
{
  vec2i_t ab = {b.x - a.x, b.y - a.y};
  vec2i_t ap = {p.x - a.x, p.y - a.y};

  return ab.x * ap.y - ab.y * ap.x; 
}

bool edge_is_top_or_left(vec2i_t start, vec2i_t end)
{
  vec2i_t edge = {end.x - start.x, end.y - start.y};

  bool is_top = edge.y == 0 && edge.x > 0;
  bool is_left = edge.y < 0;

  return is_top || is_left;
}

// provided triangles need to be clock-wise
// provided vertices are expected to be in the canvas space
void triangle_draw(vertex_t vertex0, vertex_t vertex1, vertex_t vertex2)
{
  vec2i_t v0 = canvas_to_screen((Vector2){vertex0.pos.x, vertex0.pos.y});
  vec2i_t v1 = canvas_to_screen((Vector2){vertex1.pos.x, vertex1.pos.y});
  vec2i_t v2 = canvas_to_screen((Vector2){vertex2.pos.x, vertex2.pos.y});

  int x_min = std::min({v0.x, v1.x, v2.x});
  int y_min = std::min({v0.y, v1.y, v2.y});
  int x_max = std::max({v0.x, v1.x, v2.x});
  int y_max = std::max({v0.y, v1.y, v2.y});

  // find the area of triangle
  float area = edge_cross(v0, v1, v2);

  int bias0 = edge_is_top_or_left(v0, v1) ? 0 : -1;
  int bias1 = edge_is_top_or_left(v1, v2) ? 0 : -1;
  int bias2 = edge_is_top_or_left(v2, v0) ? 0 : -1;

  for (int x = x_min; x <= x_max; x++)
  {
    for (int y = y_min; y <= y_max; y++)
    {
      vec2i_t p = {x, y};

      int w0 = edge_cross(v0, v1, p) + bias0;
      int w1 = edge_cross(v1, v2, p) + bias1;
      int w2 = edge_cross(v2, v0, p) + bias2;

      bool is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

      if (is_inside)
      {
        // barycentric weights
        float a = (float)w0 / area;
        float b = (float)w1 / area;
        float c = (float)w2 / area;

        Color color = {
          static_cast<unsigned char>(Clamp(vertex0.color.r * b + vertex1.color.r * c + vertex2.color.r * a, 0, 255)),
          static_cast<unsigned char>(Clamp(vertex0.color.g * b + vertex1.color.g * c + vertex2.color.g * a, 0, 255)),
          static_cast<unsigned char>(Clamp(vertex0.color.b * b + vertex1.color.b * c + vertex2.color.b * a, 0, 255)),
          255
        };

        DrawPixel(x, y, color);
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
  const int screenWidth = 400;
  const int screenHeight = 400;

  InitWindow(screenWidth, screenHeight, "Game");

  // The four "front" vertices
  Vector3 vAf = {-2, -0.5, 5};
  Vector3 vBf = {-2,  0.5, 5};
  Vector3 vCf = {-1,  0.5, 5};
  Vector3 vDf = {-1, -0.5, 5};

  // The four "back" vertices
  Vector3 vAb = {-2, -0.5, 6};
  Vector3 vBb = {-2,  0.5, 6};
  Vector3 vCb = {-1,  0.5, 6};
  Vector3 vDb = {-1, -0.5, 6};

  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
      BeginDrawing();
      ClearBackground(RAYWHITE);
      
      // The front face
      line_draw(ProjectVertex(vAf), ProjectVertex(vBf), BLUE);
      line_draw(ProjectVertex(vBf), ProjectVertex(vCf), BLUE);
      line_draw(ProjectVertex(vCf), ProjectVertex(vDf), BLUE);
      line_draw(ProjectVertex(vDf), ProjectVertex(vAf), BLUE);

      // The back face
      line_draw(ProjectVertex(vAb), ProjectVertex(vBb), RED);
      line_draw(ProjectVertex(vBb), ProjectVertex(vCb), RED);
      line_draw(ProjectVertex(vCb), ProjectVertex(vDb), RED);
      line_draw(ProjectVertex(vDb), ProjectVertex(vAb), RED);

      // The front-to-back edges
      line_draw(ProjectVertex(vAf), ProjectVertex(vAb), GREEN);
      line_draw(ProjectVertex(vBf), ProjectVertex(vBb), GREEN);
      line_draw(ProjectVertex(vCf), ProjectVertex(vCb), GREEN);
      line_draw(ProjectVertex(vDf), ProjectVertex(vDb), GREEN);

      EndDrawing();
  }
  CloseWindow();
  return 0;
}

#pragma endregion