#include "include/raylib.h"
#include "include/raymath.h"

typedef struct
{
  Vector3 center;
  Vector3 vertices[8];
  int numberOfVertices;
  int numberOfTriangles;
  int triangles[36];
}Cube;

Cube cube_init(Vector3 center)
{
  Cube cube;
  cube.center = center;
  cube.numberOfVertices = 8;
  cube.numberOfTriangles = 36;
  
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
  int triangles[36] = {
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

  for (int i = 0; i < cube.numberOfTriangles; i++)
  {
      cube.triangles[i] = triangles[i];
  }

  return cube;
}

void cube_draw(const Cube* cube)
{
    for (int i = 0; i < cube->numberOfTriangles; i += 3)
    {
        DrawTriangle3D(cube->vertices[cube->triangles[i]],
                       cube->vertices[cube->triangles[i+1]],
                       cube->vertices[cube->triangles[i+2]],
                    BLACK);
    }
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game");

    Cube cube = cube_init((Vector3){0, 0, 0});

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        cube_draw(&cube);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}