#pragma once

#include "../include/raylib.h"

#pragma region helpers
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

Vector4 mul_v3_mat(Vector3 v3, Matrix m)
{
  // Convert Vector3 to Vector4
  Vector4 v = { v3.x, v3.y, v3.z, 1.0f };

  // Perform matrix multiplication (column-major order)
  Vector4 result;
  result.x = v.x * m.m0 + v.y * m.m4 + v.z * m.m8  + v.w * m.m12;
  result.y = v.x * m.m1 + v.y * m.m5 + v.z * m.m9  + v.w * m.m13;
  result.z = v.x * m.m2 + v.y * m.m6 + v.z * m.m10 + v.w * m.m14;
  result.w = v.x * m.m3 + v.y * m.m7 + v.z * m.m11 + v.w * m.m15;

  return result;
}
#pragma endregion

Vector3 transform_to_world_space(Vector3 v, const transform_t& tr)
{
  Matrix s = MatrixScale(tr.scale.x, tr.scale.y, tr.scale.z);
  Matrix r = MatrixRotateZYX(tr.rotation);
  Matrix t = MatrixTranslate(tr.position.x, tr.position.y, tr.position.z);
  
  Matrix transform_matrix = MatrixMultiply(MatrixMultiply(s, r), t);

  return Vector3Transform(v, transform_matrix);
}

Vector3 transform_to_camera_space(Vector3 v_world, const camera_t& cam)
{
  Vector3 v_translated = Vector3Subtract(v_world, cam.position);

  Vector3 v_camera = Vector3Transform(v_translated, MatrixInvert(MatrixRotateZYX(cam.rot_in_rad)));

  return v_camera;
}

Vector4 apply_projection_matrix(Vector3 v, camera_t cam)
{
  return mul_v3_mat(v, get_projection_matrix(cam));
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
