#include "include/raylib.h"
#include "include/raymath.h"
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

Vector2 project_vertex(Vector3 vertex)
{
    float x = vertex.x;
    float y = vertex.y;
    float z = vertex.z;

    // Perspective division
    x /= z;
    y /= z;

    // Convert to screen coordinates
    Vector2 screen;
    //screen.x = (x  + 1.0f) * GetScreenWidth() / 2.0f;
    //screen.y = (-y + 1.0f ) * GetScreenHeight() / 2.0f;

    float w = GetScreenWidth() / 2.0f;
    float h = GetScreenHeight() / 2.0f;

    screen.x =  x * w + w;
    screen.y = -y * h + h; // negative because y is inverted in raylib
    return screen;
}

typedef struct
{
  Vector3 center;
  Vector3 vertices[8];
  int numberOfVertices;
  int numberOfTriangleIndicies;
  int triangleIndicies[36];
} Cube;

Cube cube_init(Vector3 center)
{
  Cube cube;
  cube.center = center;
  cube.numberOfVertices = 8;
  cube.numberOfTriangleIndicies = 36;
  
  // vertices 
  cube.vertices[0] = Vector3Add(center, (Vector3){-1, -1, -1});  // Front bottom left
  cube.vertices[1] = Vector3Add(center, (Vector3){ 1, -1, -1});  // Front bottom right
  cube.vertices[2] = Vector3Add(center, (Vector3){ 1,  1, -1});  // Front top right
  cube.vertices[3] = Vector3Add(center, (Vector3){-1,  1, -1});  // Front top left
  cube.vertices[4] = Vector3Add(center, (Vector3){-1, -1,  1});  // Back bottom left
  cube.vertices[5] = Vector3Add(center, (Vector3){ 1, -1,  1});  // Back bottom right
  cube.vertices[6] = Vector3Add(center, (Vector3){ 1,  1,  1});  // Back top right
  cube.vertices[7] = Vector3Add(center, (Vector3){-1,  1,  1});  // Back top left

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

    Cube cube = cube_init((Vector3){0, 0, -10}); // Negative Z value for visibility
    float angle = 0.02f;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        for (int i = 0; i < cube.numberOfVertices; i++)
        {
            cube.vertices[i] = RotatePointAroundAxis(cube.center, cube.vertices[i], (Vector3){0, 1, 0}, angle);
        }

        BeginDrawing();
        ClearBackground(WHITE);

        cube_draw(&cube);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
