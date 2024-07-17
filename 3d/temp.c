#include "include/raylib.h"
#include "include/raymath.h"
#include <stdbool.h>
#include <stdio.h>

Vector3 RotatePointAroundAxis(Vector3 center, Vector3 point, Vector3 axis, float angle) 
{
    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);

    // Translate point to origin
    float x = point.x - center.x;
    float y = point.y - center.y;
    float z = point.z - center.z;

    // Normalize the rotation axis
    float axisLength = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    float u = axis.x / axisLength;
    float v = axis.y / axisLength;
    float w = axis.z / axisLength;

    // Compute the rotated point coordinates
    float newX = (u * (u * x + v * y + w * z) * (1 - cosAngle) 
                  + x * cosAngle 
                  + (-w * y + v * z) * sinAngle);
    
    float newY = (v * (u * x + v * y + w * z) * (1 - cosAngle) 
                  + y * cosAngle 
                  + (w * x - u * z) * sinAngle);
    
    float newZ = (w * (u * x + v * y + w * z) * (1 - cosAngle) 
                  + z * cosAngle 
                  + (-v * x + u * y) * sinAngle);

    // Translate the point back
    Vector3 rotatedPoint = {newX, newY, newZ};
    rotatedPoint = Vector3Add(center, rotatedPoint);

    return rotatedPoint;
}

typedef struct
{
  Vector3 position;
  Vector3 lookAtPoint;
  Vector3 upVector;
}myCam;

Matrix myGetCameraViewMatrix(myCam *cam)
{
  Vector3 forward = Vector3Normalize(Vector3Subtract(cam->position, cam->lookAtPoint));
  Vector3 right = Vector3Normalize(Vector3CrossProduct(cam->upVector, forward));
  Vector3 up = Vector3CrossProduct(forward, right);

  Matrix r = {
    right.x, right.y, right.z, 0,
    up.x, up.y, up.z, 0,
    forward.x, forward.y, forward.z, 0,
    0, 0, 0, 1
  };

  Matrix t = {
    1, 0, 0, -cam->position.x,
    0, 1, 0, -cam->position.y,
    0, 0, 1, -cam->position.z,
    0, 0, 0, 1
  };

  return MatrixMultiply(r, t);
}

Vector3 MultiplyMatrixVector(Matrix mat, Vector3 vec)
{
    Vector3 result;
    result.x = mat.m0 * vec.x + mat.m4 * vec.y + mat.m8 * vec.z + mat.m12;
    result.y = mat.m1 * vec.x + mat.m5 * vec.y + mat.m9 * vec.z + mat.m13;
    result.z = mat.m2 * vec.x + mat.m6 * vec.y + mat.m10 * vec.z + mat.m14;
    return result;
}

Vector2 project_vertex(Vector3 vertex)
{
    float x = vertex.x;
    float y = vertex.y;
    float z = vertex.z;

    // Perspective division
    x /= -z;
    y /= -z;

    float x_proj_remap = (1 + x) / 2;
    float y_proj_remap = (1 + y) / 2;

    float x_proj_pix = x_proj_remap * GetScreenWidth();
    float y_proj_pix = y_proj_remap * GetScreenHeight();

    return (Vector2){x_proj_pix, y_proj_pix};    
}

typedef struct
{
  Vector3 center;
  Vector3 vertices[8];
  int numberOfVertices;
  int numberOfTriangleIndicies;
  int triangleIndicies[36];
} Cube;

Cube cube_init(Vector3 center, float scale)
{
  Cube cube;
  cube.center = center;
  cube.numberOfVertices = 8;
  cube.numberOfTriangleIndicies = 36;
  
  // vertices 
  cube.vertices[0] = Vector3Add(center, (Vector3){-1 * scale, -1 * scale, -1 * scale});  // Front bottom left
  cube.vertices[1] = Vector3Add(center, (Vector3){ 1 * scale, -1 * scale, -1 * scale});  // Front bottom right
  cube.vertices[2] = Vector3Add(center, (Vector3){ 1 * scale,  1 * scale, -1 * scale});  // Front top right
  cube.vertices[3] = Vector3Add(center, (Vector3){-1 * scale,  1 * scale, -1 * scale});  // Front top left
  cube.vertices[4] = Vector3Add(center, (Vector3){-1 * scale, -1 * scale,  1 * scale});  // Back bottom left
  cube.vertices[5] = Vector3Add(center, (Vector3){ 1 * scale, -1 * scale,  1 * scale});  // Back bottom right
  cube.vertices[6] = Vector3Add(center, (Vector3){ 1 * scale,  1 * scale,  1 * scale});  // Back top right
  cube.vertices[7] = Vector3Add(center, (Vector3){-1 * scale,  1 * scale,  1 * scale});  // Back top left

  // triangles
  int triangleIndicies[36] = {
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Right face
    1, 5, 6,
    6, 2, 1,
    // Back face
    5, 4, 7,
    7, 6, 5,
    // Left face
    4, 0, 3,
    3, 7, 4,
    // Top face
    3, 2, 6,
    6, 7, 3,
    // Bottom face
    4, 5, 1,
    1, 0, 4
  };

  for (int i = 0; i < cube.numberOfTriangleIndicies; i++)
  {
      cube.triangleIndicies[i] = triangleIndicies[i];
  }

  return cube;
}

void cube_draw(const Cube* cube)
{
    for (int i = 0; i < cube->numberOfTriangleIndicies; i += 3)
    {
      Vector2 v1 = project_vertex(cube->vertices[cube->triangleIndicies[i  ]]);
      Vector2 v2 = project_vertex(cube->vertices[cube->triangleIndicies[i+1]]);
      Vector2 v3 = project_vertex(cube->vertices[cube->triangleIndicies[i+2]]);

      DrawTriangleLines(v1, v2, v3, BLACK);
    }
}

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "3D Cube");

    Cube cube = cube_init((Vector3){0, 0, 0},  1); // Negative Z value for visibility

    myCam cam;
    cam.position = (Vector3){0, 0, 10};
    cam.lookAtPoint = (Vector3){0, 0, 0};
    cam.upVector = (Vector3){0, 1, 0};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

      if (IsKeyDown(KEY_A))
      {
        cam.position = Vector3Add(cam.position, (Vector3){-0.1, 0, 0});
      }

      if (IsKeyDown(KEY_D))
      {
        cam.position = Vector3Add(cam.position, (Vector3){0.1, 0, 0});
      }

      if (IsKeyDown(KEY_W))
      {
        cam.position = Vector3Add(cam.position, (Vector3){0, 0.1, 0});
      }

      if (IsKeyDown(KEY_S))
      {
        cam.position = Vector3Add(cam.position, (Vector3){0, -0.1, 0});
      }

      Matrix viewMatrix = myGetCameraViewMatrix(&cam);

       Vector3 transformedVertices[8];
      for (int i = 0; i < cube.numberOfVertices; i++)
      {
        transformedVertices[i] = MultiplyMatrixVector(viewMatrix, cube.vertices[i]);
      }

      BeginDrawing();
      ClearBackground(WHITE);

      for (int i = 0; i < cube.numberOfTriangleIndicies; i += 3)
      {
          Vector2 v1 = project_vertex(transformedVertices[cube.triangleIndicies[i]]);
          Vector2 v2 = project_vertex(transformedVertices[cube.triangleIndicies[i+1]]);
          Vector2 v3 = project_vertex(transformedVertices[cube.triangleIndicies[i+2]]);

          DrawTriangleLines(v1, v2, v3, BLACK);
      }

      EndDrawing();
    }
    CloseWindow();
    return 0;
}
