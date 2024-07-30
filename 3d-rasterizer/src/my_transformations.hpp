#pragma once

#include "../include/raylib.h"
#include "my_utils.hpp"
#include "my_globals.hpp"

#pragma region canvas

vec2i_t canvas_to_screen(int Cx, int Cy)
{
  int screen_x = (GetScreenWidth() / 2)  + Cx;
  int screen_y = (GetScreenHeight() / 2) - Cy;

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

  return (Vector2){x * (Cw / Vw), y * (Ch / Vh)};
}

#pragma endregion

Vector3 apply_transform(Vector3 v, const transform_t& tr)
{
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

Vector3 apply_perspective(Vector3 v_cam_space)
{
  float d = VIEWPORT_DISTANCE_TO_CAMERA;
  Vector3 v = v_cam_space;
  
  return (Vector3){
      v.x * d / v.z, 
      v.y * d / v.z,
      v.z
    };
}
