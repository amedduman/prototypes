#pragma once

#include "my_model.hpp"
#include "my_camera.hpp"
#include "my_transformations.hpp"
#include <vector>
#include <iostream>

void triangle_wireframe_draw(Vector2 p0, Vector2 p1, Vector2 p2, Color color)
{
  DrawLineV(p0,p1, color);
  DrawLineV(p1, p2, color);
  DrawLineV(p2, p0, color);
}

void triangle_draw(Vector2 p0, Vector2 p1, Vector2 p2, Color color)
{
  // my rendering system is draw's triangles clock-wise and this function expects them in counter-clock-wise so I enter the points in reversed order.
  DrawTriangle(
    p2,
    p1,
    p0,
    color
  );
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

void render_triangle(const triangle_t& triangle, const std::vector<Vector2>& projected_vertices, const std::vector<Vector3>& cam_space_verts)
{
  /*
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
    return;
  }
  */

  if(is_back_face(triangle, cam_space_verts)) return;

  triangle_draw(
    projected_vertices[triangle.tri_indices[0]],
    projected_vertices[triangle.tri_indices[1]],
    projected_vertices[triangle.tri_indices[2]],
    triangle.color
  );
}

void render_model_instance(const instance_t& instance, const camera_t& cam)
{
  std::vector<Vector2> protected_vertices = {};
  std::vector<Vector3> camera_space_vertices = {};
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
    render_triangle(instance.model.triangles[i], protected_vertices, camera_space_vertices);
  }
}

void render_scene(std::vector<instance_t> scene, camera_t cam)
{
  for (size_t i = 0; i < scene.size(); i++)
  {
    render_model_instance(scene[i], cam);
  }
}
