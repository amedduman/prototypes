#include "include/raylib.h"
#include "include/raymath.h"

typedef struct {
  float fov;
  float aspect;
  float near;
  float far;
} myCamera;

Vector2 project_vertex(Vector3 vertex, const myCamera* camera)
{
    // Simple perspective projection
    float fov_rad = camera->fov * PI / 180.0f;
    float scale = 1.0f / tanf(fov_rad / 2.0f);
    
    // Apply perspective projection
    float x = vertex.x * scale / camera->aspect;
    float y = vertex.y * scale;
    float z = (vertex.z * (camera->far + camera->near) / (camera->far - camera->near) + 
               (2 * camera->far * camera->near) / (camera->far - camera->near)) / -vertex.z;

    // Perspective division
    x /= z;
    y /= z;

    // Convert to screen coordinates
    Vector2 screen;
    screen.x = (x + 1.0f) * GetScreenWidth() / 2.0f;
    screen.y = (1.0f - y) * GetScreenHeight() / 2.0f;

    return screen;
}

typedef struct
{
  Vector3 center;
  Vector3 vertices[8];
  int numberOfVertices;
  int numberOfTriangles;
  int triangleIndicies[36];
} Cube;

Cube cube_init(Vector3 center)
{
  Cube cube;
  cube.center = center;
  cube.numberOfVertices = 8;
  cube.numberOfTriangles = 12;
  
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

  for (int i = 0; i < cube.numberOfTriangles * 3; i++)
  {
      cube.triangleIndicies[i] = triangleIndicies[i];
  }

  return cube;
}

void cube_draw(const Cube* cube, const myCamera* camera)
{
    for (int i = 0; i < cube->numberOfTriangles; i += 3)
    {
      Vector2 v1 = project_vertex(cube->vertices[cube->triangleIndicies[i  ]], camera);
      Vector2 v2 = project_vertex(cube->vertices[cube->triangleIndicies[i+1]], camera);
      Vector2 v3 = project_vertex(cube->vertices[cube->triangleIndicies[i+2]], camera);

      DrawTriangleLines(v1, v2, v3, RED);
      //DrawTriangle(v1, v2, v3, RED);
    }
}

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "Game");

    myCamera camera = {
      .fov = 100.0f,
      .aspect = (float)screenWidth / (float)screenHeight,
      .near = 0.1f,
      .far = 100.0f
    };

    Cube cube = cube_init((Vector3){0, 0, -10});

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        cube_draw(&cube, &camera);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}