#pragma once

#include "my_model.hpp"
#include "my_drawing.hpp"
#include "my_camera.hpp"
#include <vector>

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
