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

void render_triangle(const triangle_t& triangle, const std::vector<Vector2>& projected_vertices)
{
  triangle_wireframe_draw(
      projected_vertices[triangle.tri_indices[0]],
      projected_vertices[triangle.tri_indices[1]],
      projected_vertices[triangle.tri_indices[2]],
      triangle.color
    );
}

void render_model_instance(const instance_t& instance, const camera_t& cam)
{
  std::vector<Vector2> protected_vertices = {};
  for (size_t i = 0; i < instance.model.vertices.size(); i++)
  {
    Vector3 v_world = transform_to_world_space(instance.model.vertices[i], instance.transform);
   
    Vector3 v_camera = transform_to_camera_space(v_world, cam);

    Vector4 v_proj_applied = apply_projection_matrix(v_camera, cam);
    
    Vector3 v_perspective_applied = apply_perspective_division(v_proj_applied);

    Vector2 v_screen = map_ndc_to_screen(v_perspective_applied);

    protected_vertices.push_back(v_screen);
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
