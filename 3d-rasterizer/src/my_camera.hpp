#pragma once

#include "../include/raylib.h"

#define CAM_SPEED 0.3f

typedef struct
{
  Vector3 position;
  Vector3 rot_in_rad;
} camera_t;

void camera_move(camera_t& cam)
{
  Vector3 m = {0};

  if (IsKeyDown(KEY_A))m.x = -1;
  if (IsKeyDown(KEY_D))m.x = 1;
  if (IsKeyDown(KEY_W))m.y = 1;
  if (IsKeyDown(KEY_S))m.y = -1;

  m = Vector3Scale(m, CAM_SPEED);
  cam.position = Vector3Add(cam.position, m);
}
