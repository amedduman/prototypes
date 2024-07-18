#include "include/raylib.h"
#include "include/raymath.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
  Vector3 position;
  Vector3 lookAtPoint;
  Vector3 upVector;
  float fov;
  float aspect;
  float near;
  float far;
}myCam;

Matrix myGetCameraViewMatrix(const myCam *cam)
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

Vector2 project_vertex(Vector3 vertex, const myCam* cam)
{
    float x = vertex.x;
    float y = vertex.y;
    float z = vertex.z;

    float fovRad = cam->fov * DEG2RAD;
    float tanHalfFov = tanf(fovRad / 2.0f);

    // multiplying values to adjust them according to fov and aspect ratio of screen
    x = x * tanHalfFov * cam->aspect;
    y = y * tanHalfFov;

    // Perspective division
    x = x / z;
    y = y / z;

    // after vertices being transformed to camera space and being divide by z for perpective they are 
    // end up being between -1,1 which is we called as normalized device space
    // so we are going to map them between 0,1 to make calculations easier for ourselves
    // and since y is reversed in screen space we are going to do 1 - y instead of 1 + y
    float x_proj_remap = (1 + x) * 0.5f ;
    float y_proj_remap = (1 - y) * 0.5f;

    // after mapping we are going to multiply them with screen width and height
    // to get pixel cordinate of the point 
    float x_proj_pix = x_proj_remap * GetScreenWidth();
    float y_proj_pix = y_proj_remap * GetScreenHeight();

    return (Vector2){x_proj_pix, y_proj_pix};    
}

// REMEMBER -> the incomin point is in the camera space
Vector2 manual_perspective_projection(const Vector3 point, const myCam* cam)
{
  // Check if the point is behind the camera
  if (point.z >= 0)
  {
    printf("Point is behind the camera: z = %f\n", point.z);
    return (Vector2){-1, -1};  // Return an invalid point
  }

    // Convert FOV to radians
    float fovRad = cam->fov * DEG2RAD;

    
    // Calculate the cotangent of half the FOV
    float cotHalfFov = 1.0f / tanf(fovRad * 0.5f);

    // perspective projection ↓

    // apply FOV
    float x_proj = point.x * cotHalfFov;
    float y_proj = point.y * cotHalfFov;

    // Apply aspect ratio correction to x
    x_proj *= cam->aspect;
    
    // Apply perspective division
    x_proj = point.x / point.z;
    y_proj = point.y / point.z;
    
    // mapping ↓
    
    // Map z to [0, 1] range
    float z_proj = (cam->far + cam->near) / (cam->far - cam->near) + (-2 * cam->far * cam->near) / ((cam->far - cam->near) * point.z);
    
    // Clip if outside the near and far planes
    if (fabsf(point.z) < cam->near || fabsf(point.z) > cam->far) 
    {
      printf("this point is outside the near and far planes\n");
      printf("point.z: %f\n", fabsf(point.z));
      return (Vector2){-1, -1};  // Return an invalid point
    }
    
    // Map to screen space (assuming [-1, 1] to [0, screenWidth/Height])
    float x_screen = (x_proj + 1.0f) * 0.5f * GetScreenWidth();
    float y_screen = (1.0f - y_proj) * 0.5f * GetScreenHeight();
    
    return (Vector2){x_screen, y_screen};
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

void cube_draw(const Cube* cube, const myCam* cam)
{
  Matrix viewMatrix = myGetCameraViewMatrix(cam);
  for (int i = 0; i < cube->numberOfTriangleIndicies; i += 3)
  {
    Vector3 v1 = MultiplyMatrixVector(viewMatrix, cube->vertices[cube->triangleIndicies[i]]);
    Vector3 v2 = MultiplyMatrixVector(viewMatrix, cube->vertices[cube->triangleIndicies[i+1]]);
    Vector3 v3 = MultiplyMatrixVector(viewMatrix, cube->vertices[cube->triangleIndicies[i+2]]);

    Vector2 p1 = manual_perspective_projection(v1, cam);
    Vector2 p2 = manual_perspective_projection(v2, cam);
    Vector2 p3 = manual_perspective_projection(v3, cam);

    DrawTriangleLines(p1, p2, p3, BLACK);
  }
}

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "3D Cube");

    Cube cube = cube_init((Vector3){0, 0, 0},  1);

    myCam cam = {
      .position = (Vector3){0, 0, 10},
      .lookAtPoint = (Vector3){0, 0, 0},
      .upVector = (Vector3){0, 1, 0},
      .aspect = (float)screenWidth / (float)screenHeight,
      .fov = 60.0f,
      .near = 1,
      .far = 100
    };

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

      if (IsKeyDown(KEY_A)) cam.position.x += 0.1f;
      if (IsKeyDown(KEY_D)) cam.position.x -= 0.1f;
      if (IsKeyDown(KEY_W)) cam.position.y -= 0.1f;
      if (IsKeyDown(KEY_S)) cam.position.y += 0.1f;
      if (IsKeyDown(KEY_Q)) cam.position.z -= 0.1f;
      if (IsKeyDown(KEY_E)) cam.position.z += 0.1f;

      Matrix viewMatrix = myGetCameraViewMatrix(&cam);

       Vector3 transformedVertices[8];
      for (int i = 0; i < cube.numberOfVertices; i++)
      {
        transformedVertices[i] = MultiplyMatrixVector(viewMatrix, cube.vertices[i]);
      }

      BeginDrawing();
      ClearBackground(WHITE);

      cube_draw(&cube, &cam);

      EndDrawing();
    }
    CloseWindow();
    return 0;
}
