#pragma once

#include "../include/raylib.h"
#include <vector>

typedef struct
{
  int tri_indices[3];
  Color color;
} triangle_t;


typedef struct
{
  std::vector<Vector3> vertices;
  std::vector<triangle_t> triangles;
  std::vector<Vector2> uv_of_each_vertex;
  std::vector<Color> colors;
} model_t;

typedef struct
{
  Vector3 position;
  Vector3 rotation;
  Vector3 scale;
} transform_t;


typedef struct
{
  model_t model;
  transform_t transform;

} instance_t;

instance_t model_init_instance(const model_t& model, Vector3 pos, Vector3 rot_in_rad, Vector3 my_scale)
{
  instance_t instance = {
    .model = model,
    .transform = {
      .position = pos,
      .rotation = rot_in_rad,
      .scale = my_scale
    }
  };

  return instance;
}