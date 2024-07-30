#include "../include/raylib.h"
#include "../include/raymath.h"
#include "my_utils.hpp"
#include "my_model.hpp"
#include "my_camera.hpp"
#include <iostream>
#include <assert.h>
#include <vector>

#define VIEWPORT_HEIGHT 1.0f
#define VIEWPORT_WIDTH 1.0f
#define VIEWPORT_DISTANCE_TO_CAMERA 1.0f

#pragma region canvas - viewport

void canvas_put_pixel(int Cx, int Cy, Color color)
{
  int Sx = GetScreenWidth() / 2 + Cx;
  int Sy = GetScreenHeight() / 2 - Cy;

  DrawPixel(Sx, Sy, color);
}

Vector3 canvas_to_viewport(int Cx, int Cy)
{
  float Vw = VIEWPORT_WIDTH;
  float Vh = VIEWPORT_HEIGHT;
  float d = VIEWPORT_DISTANCE_TO_CAMERA;

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

  float Vw = VIEWPORT_WIDTH;
  float Vh = VIEWPORT_HEIGHT;
  //float d = VIEWPORT_DISTANCE_TO_CAMERA;

  return (Vector2){x * (Cw / Vw), y * (Ch / Vh)};
}

#pragma endregion

#pragma region projection

Vector2 project_vertex(Vector3 v)
{ 
  float d = VIEWPORT_DISTANCE_TO_CAMERA;
  
  return viewport_to_canvas(v.x * d / v.z, v.y * d / v.z);
}

#pragma endregion

#pragma region line drawing

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

#pragma region rotation

Vector3 rotate_y(Vector3 v, float angle_in_rad)
{
  float x = v.x * std::cos(angle_in_rad) + v.z * std::sin(angle_in_rad);
  float z = -v.x * std::sin(angle_in_rad) + v.z * std::cos(angle_in_rad);

  return (Vector3){x, v.y, z}; 
}

#pragma endregion

#pragma region rendering

void render_triangle(const triangle_t& triangle, const std::vector<Vector2>& projected_vertices)
{
  triangle_wireframe_draw(
      projected_vertices[triangle.tri_indices[0]],
      projected_vertices[triangle.tri_indices[1]],
      projected_vertices[triangle.tri_indices[2]],
      triangle.color
    );
}

void render_model(const model_t& m)
{
  std::vector<Vector2> protected_vertices = {};
  for (size_t i = 0; i < m.vertices.size(); i++)
  {
    protected_vertices.push_back(project_vertex(m.vertices[i]));
  }

  for (size_t i = 0; i < m.triangles.size(); i++)
  {
    render_triangle(m.triangles[i], protected_vertices);
  }
}

// transform from model space to world space
Vector3 apply_transform(Vector3 v, const transform_t& tr)
{
  //Vector3 result = {0,0,0};
  
  Matrix s = MatrixScale(tr.scale.x, tr.scale.y, tr.scale.z);
  Matrix r = MatrixRotateZYX(tr.rotation);
  Matrix t = MatrixTranslate(tr.position.x, tr.position.y, tr.position.z);
  
  Matrix transform_matrix = MatrixMultiply(MatrixMultiply(s, r), t);

  return Vector3Transform(v, transform_matrix);
}

Vector3 apply_camera_transform(Vector3 v_world, const camera_t& cam)
{
  Vector3 v_translated = Vector3Subtract(v_world, cam.position);

  Vector3 v_camera = Vector3Transform(v_translated, MatrixInvert(MatrixRotateZYX(cam.rot_in_rad)));

  return v_camera;
}

void render_model_instance(const instance_t& instance, const camera_t& cam)
{
  std::vector<Vector2> protected_vertices = {};
  for (size_t i = 0; i < instance.model.vertices.size(); i++)
  {
    // transform from model space to world space
    Vector3 v_world = apply_transform(instance.model.vertices[i], instance.transform);
    // transform form world space to camera space
    Vector3 v_camera = apply_camera_transform(v_world, cam);

    
    protected_vertices.push_back(project_vertex(v_camera));
  }

  for (size_t i = 0; i < instance.model.triangles.size(); i++)
  {
    render_triangle(instance.model.triangles[i], protected_vertices);
  }
}

void render_scene(std::vector<instance_t> scene, camera_t cam)
{
  for (size_t i = 0; i < scene.size(); i++)
  {
    render_model_instance(scene[i], cam);
  }
}

#pragma endregion

#pragma region main

int main(void)
{
  const int screenWidth = 400;
  const int screenHeight = 400;

  SetTraceLogLevel(LOG_WARNING); 

  InitWindow(screenWidth, screenHeight, "Game");

  model_t cube = {
    
      .vertices = {
      (Vector3){ 1,  1,  1},
      (Vector3){-1,  1,  1},
      (Vector3){-1, -1,  1},
      (Vector3){ 1, -1,  1},
      (Vector3){ 1,  1, -1},
      (Vector3){-1,  1, -1},
      (Vector3){-1, -1, -1},
      (Vector3){ 1, -1, -1},
    },
    .triangles = {
      {.tri_indices = {0, 1, 2}, .color = RED},
      {.tri_indices = {0, 2, 3}, .color = RED},
      {.tri_indices = {4, 0, 3}, .color = GREEN},
      {.tri_indices = {4, 3, 7}, .color = GREEN},
      {.tri_indices = {5, 4, 7}, .color = BLUE},
      {.tri_indices = {5, 7, 6}, .color = BLUE},
      {.tri_indices = {1, 5, 6}, .color = GOLD},
      {.tri_indices = {1, 6, 2}, .color = GOLD},
      {.tri_indices = {4, 5, 1}, .color = PURPLE},
      {.tri_indices = {4, 1, 0}, .color = PURPLE},
      {.tri_indices = { 2, 6, 7},.color = BLACK},
      {.tri_indices = { 2, 7, 3},.color = BLACK},
    }

  };

  instance_t cube_a = model_init_instance(cube, (Vector3){0, 0, 12}, (Vector3){0, DEG2RAD * 0, 0}, (Vector3){2,1,1});

  camera_t camera = {
    .position = {0,0,1},
    .rot_in_rad = (Vector3){0, DEG2RAD * 30, 0}
  };

  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    render_scene(std::vector<instance_t>{cube_a}, camera);

    EndDrawing();
  }

  CloseWindow();
}

#pragma endregion