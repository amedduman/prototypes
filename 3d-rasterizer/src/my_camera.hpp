#pragma once

#include "../include/raylib.h"

#define CAM_MOVE_SPEED 0.3f
#define CAM_ROT_SPEED 2

typedef struct
{
  Vector3 position;
  Vector3 rot_in_rad;
  float fov;
  float z_near;
  float z_far;
} camera_t;

void camera_move(camera_t& cam)
{
  Vector3 m = {0};

  if (IsKeyDown(KEY_A))m.x = -1;
  if (IsKeyDown(KEY_D))m.x = 1;
  if (IsKeyDown(KEY_W))m.y = 1;
  if (IsKeyDown(KEY_S))m.y = -1;

  m = Vector3Scale(m, CAM_MOVE_SPEED);
  cam.position = Vector3Add(cam.position, m);
}

void camera_rotate(camera_t& cam)
{
  float s = CAM_ROT_SPEED;
  if (IsKeyDown(KEY_LEFT))  cam.rot_in_rad = Vector3Add(cam.rot_in_rad, (Vector3){DEG2RAD*0, -DEG2RAD*s, DEG2RAD*0});
  if (IsKeyDown(KEY_RIGHT)) cam.rot_in_rad = Vector3Add(cam.rot_in_rad, (Vector3){DEG2RAD*0,  DEG2RAD*s, DEG2RAD*0});
  if (IsKeyDown(KEY_UP))    cam.rot_in_rad = Vector3Add(cam.rot_in_rad, (Vector3){-DEG2RAD*s,  DEG2RAD*0, DEG2RAD*0});
  if (IsKeyDown(KEY_DOWN))  cam.rot_in_rad = Vector3Add(cam.rot_in_rad, (Vector3){DEG2RAD*s,  DEG2RAD*0, DEG2RAD*0});
}