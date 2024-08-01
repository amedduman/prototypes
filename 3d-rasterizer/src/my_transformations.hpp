#pragma once

#include "../include/raylib.h"
#include "my_utils.hpp"
#include "my_globals.hpp"
#include <iostream>

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

Matrix get_projection_matrix(const camera_t& cam)
{
  float a = (float)GetScreenHeight() / (float)GetScreenWidth();;
  float fov = cam.fov * DEG2RAD;
  float f = 1.0f / (tanf(fov / 2.0f));
  float z1 = cam.z_far / (cam.z_far - cam.z_near);
  float z2 = -(cam.z_far / (cam.z_far - cam.z_near)) * cam.z_near;
  
  Matrix proj = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
  };

  proj.m0 = a*f;
  proj.m5 = f;
  proj.m10 = z1;
  proj.m14 = z2;
  proj.m11 = 1;

  return proj;
}

Vector3 apply_perspective_division(Vector4 v)
{
  Vector3 result = {0};

  result.x = v.x / v.w; 
  result.y = v.y / v.w; 
  result.z = v.z / v.w; 

  return result;
}

Vector2 map_ndc_to_screen(Vector3 v_ndc)
{
  //Scale from [-1, 1] to [0, 1];
  float screen_x = (v_ndc.x + 1) / 2;
  float screen_y = (1 - v_ndc.y) / 2; // Flip Y-axis
  
  // Scale to screen dimensions
  screen_x *= GetScreenWidth();
  screen_y *= GetScreenHeight();

  return {screen_x, screen_y};
}

Vector4 multiply_vector3_with_projection_matrix(Vector3 v, Matrix proj)
{
  // Convert Vector3 to Vector4
  Vector4 v4 = { v.x, v.y, v.z, 1.0f };

  // Perform matrix multiplication (column-major order)
  Vector4 result;
  result.x = v4.x * proj.m0 + v4.y * proj.m4 + v4.z * proj.m8  + v4.w * proj.m12;
  result.y = v4.x * proj.m1 + v4.y * proj.m5 + v4.z * proj.m9  + v4.w * proj.m13;
  result.z = v4.x * proj.m2 + v4.y * proj.m6 + v4.z * proj.m10 + v4.w * proj.m14;
  result.w = v4.x * proj.m3 + v4.y * proj.m7 + v4.z * proj.m11 + v4.w * proj.m15;

  return result;
}
