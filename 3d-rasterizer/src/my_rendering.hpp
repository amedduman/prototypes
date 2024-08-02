#pragma once

#include "my_model.hpp"
#include "my_camera.hpp"
#include "my_transformations.hpp"
#include <vector>
#include <iostream>

typedef struct
{
  int x;
  int y;
} vec2i_t;

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

Image crateTexture = LoadImage("/Users/amedduman/Documents/projects/raylib-C/3d-rasterizer/crate.png");
Color* crateColors = LoadImageColors(crateTexture);

// provided triangles need to be clock-wise
void triangle_draw2(const triangle_t& triangle, const std::vector<Vector3> camera_space_vertices, const std::vector<Vector2>& screen_vertices, std::vector<float>& z_buffer, const instance_t& model)
{
  vec2i_t v0 = {(int)screen_vertices[triangle.tri_indices[0]].x, (int)screen_vertices[triangle.tri_indices[0]].y};
  vec2i_t v1 = {(int)screen_vertices[triangle.tri_indices[1]].x, (int)screen_vertices[triangle.tri_indices[1]].y};
  vec2i_t v2 = {(int)screen_vertices[triangle.tri_indices[2]].x, (int)screen_vertices[triangle.tri_indices[2]].y};

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

        float depth = camera_space_vertices[0].z * b + camera_space_vertices[1].z * c + camera_space_vertices[2].z * a;


        Color texelColor = BLUE;

        if (triangle.color.r == BLUE.r && triangle.color.g == BLUE.g && triangle.color.b == BLUE.b && triangle.color.a == BLUE.a)
        {
          float t_u = model.model.uv_of_each_vertex[0].x * b
                  + model.model.uv_of_each_vertex[1].x * c 
                  + model.model.uv_of_each_vertex[2].x * a;

          float t_v = model.model.uv_of_each_vertex[0].y * b
                  + model.model.uv_of_each_vertex[1].y * c 
                  + model.model.uv_of_each_vertex[2].y * a;
          
          t_u = Clamp(t_u, 0, 1);
          t_v = Clamp(t_v, 0, 1);


          int tex_x = (int)(t_u * (crateTexture.width - 1));
          int tex_y = (int)(t_v * (crateTexture.height - 1));

          // Sample the texture
          int index = tex_y * crateTexture.width + tex_x;
          texelColor = crateColors[index];
        }
        

        if (depth < z_buffer[y * GetScreenWidth() + x])
        {
          z_buffer[y * GetScreenWidth() + x] = depth;
          DrawPixel(x, y, texelColor);
        }
      }
    }
  }
}

bool is_back_face(const triangle_t& triangle, const std::vector<Vector3>& cam_space_verts)
{
  Vector3 triangle_position = Vector3Scale(Vector3Add(Vector3Add(
      cam_space_verts[triangle.tri_indices[0]],
      cam_space_verts[triangle.tri_indices[1]]),
      cam_space_verts[triangle.tri_indices[2]]),
      1.0f / 3.0f
  );

  // get point_to_cam vector
  Vector3 v = Vector3Negate(triangle_position); // in the cam space cam is in origin, no need to substract cam's position to get the vector.

  // clock wise order
  Vector3 a = cam_space_verts[triangle.tri_indices[0]];
  Vector3 b = cam_space_verts[triangle.tri_indices[1]];
  Vector3 c = cam_space_verts[triangle.tri_indices[2]];

  // get normal
  Vector3 ab = Vector3Subtract(b, a);
  Vector3 ac = Vector3Subtract(c, a);
  Vector3 n = Vector3CrossProduct(ab, ac);

  float bf = Vector3DotProduct(v, n);

  // if dot product is negetive between normal of the surface and vector to camera then don't draw it because it is a back-face triangle
  if (bf <= 0)
  {
    return true;
  }
  return false;
}

void render_model_instance(const instance_t& instance, const camera_t& cam, std::vector<float>& z_buffer)
{
  using namespace std;
  vector<Vector2> protected_vertices = {};
  vector<Vector3> camera_space_vertices = {};
  for (size_t i = 0; i < instance.model.vertices.size(); i++)
  {
    Vector3 v_world = transform_to_world_space(instance.model.vertices[i], instance.transform);
   
    Vector3 v_camera = transform_to_camera_space(v_world, cam);
    camera_space_vertices.push_back(v_camera);

    Vector4 v_proj_applied = apply_projection_matrix(v_camera, cam);
    
    Vector3 v_perspective_applied = apply_perspective_division(v_proj_applied);

    Vector2 v_screen = map_ndc_to_screen(v_perspective_applied);

    protected_vertices.push_back(v_screen);
  }

  for (size_t i = 0; i < instance.model.triangles.size(); i++)
  {
    if(is_back_face(instance.model.triangles[i], camera_space_vertices)) continue;
  
    triangle_draw2(instance.model.triangles[i], camera_space_vertices, protected_vertices, z_buffer, instance);
  }
}

void render_scene(std::vector<instance_t> scene, camera_t cam)
{
  using namespace std;
  vector<float> z_buffer;
  int size = GetScreenWidth() * GetScreenHeight();
  for (int i = 0; i < size; i++)
  {
    z_buffer.push_back(INFINITY);
  }
  

  for (size_t i = 0; i < scene.size(); i++)
  {
    render_model_instance(scene[i], cam, z_buffer);
  }
}
