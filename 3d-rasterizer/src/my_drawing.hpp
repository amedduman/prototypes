#pragma once

#include "../include/raylib.h"
#include "../include/raymath.h"
#include "my_utils.hpp"
#include "my_transformations.hpp"
#include "my_model.hpp"
#include <algorithm>

void canvas_put_pixel(int Cx, int Cy, Color color)
{
  //int Sx = GetScreenWidth() / 2 + Cx;
  //int Sy = GetScreenHeight() / 2 - Cy;

  vec2i_t s = canvas_to_screen(Cx, Cy);

  DrawPixel(s.x, s.y, color);
}

#pragma region line drawing

/*
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
*/
#pragma endregion

#pragma region draw triangle

void triangle_wireframe_draw(Vector2 p0, Vector2 p1, Vector2 p2, Color color)
{
  DrawLineV(p0,p1, color);
  DrawLineV(p1, p2, color);
  DrawLineV(p2, p0, color);
  /*
  line_draw(p0, p1, color);
  line_draw(p1, p2, color);
  line_draw(p2, p0, color);
  */
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