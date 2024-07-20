// #region Includes
#include "include/raylib.h"
#include "include/raymath.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// #endregion

// #region Camera
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
// #endregion

// #region Matrix
Vector3 MultiplyMatrixVector(Matrix mat, Vector3 vec)
{
    Vector3 result;
    result.x = mat.m0 * vec.x + mat.m4 * vec.y + mat.m8  * vec.z + mat.m12;
    result.y = mat.m1 * vec.x + mat.m5 * vec.y + mat.m9  * vec.z + mat.m13;
    result.z = mat.m2 * vec.x + mat.m6 * vec.y + mat.m10 * vec.z + mat.m14;
    return result;
}

Vector4 MultiplyMatrixVector4(Matrix mat, Vector4 vec)
{
    Vector4 result;
    result.x = mat.m0 * vec.x + mat.m4 * vec.y + mat.m8  * vec.z + mat.m12;
    result.y = mat.m1 * vec.x + mat.m5 * vec.y + mat.m9  * vec.z + mat.m13;
    result.z = mat.m2 * vec.x + mat.m6 * vec.y + mat.m10 * vec.z + mat.m14;
    result.w = mat.m3 * vec.x + mat.m7 * vec.y + mat.m11 * vec.z + mat.m15;
    return result;
}

Vector4 Vector4FromVector3(Vector3 vec, float w)
{
    Vector4 result;
    result.x = vec.x;
    result.y = vec.y;
    result.z = vec.z;
    result.w = w;
    return result;
}
// #endregion

// #region Projection
Matrix CreatePerspectiveProjectionMatrix(float fovY, float aspect, float near, float far)
{
    float tanHalfFovY = tanf(fovY * 0.5f * DEG2RAD);
    float f = 1.0f / tanHalfFovY;
    float nf = 1.0f / (near - far);

    Matrix result = {
        f / aspect, 0.0f,  0.0f,                         0.0f,
        0.0f,       f,     0.0f,                         0.0f,
        0.0f,       0.0f,  (far + near) * nf,           -1.0f,
        0.0f,       0.0f,  2.0f * far * near * nf,       0.0f
    };

    return result;
}

// REMEMBER -> the incoming point is in the camera space
Vector2 perspective_projection(Vector3 vertex, const myCam* cam)
{
  // Create projection matrix
  Matrix projectionMatrix = CreatePerspectiveProjectionMatrix(cam->fov, cam->aspect, cam->near, cam->far);

  // Apply projection matrix
  // when we do the projection matrix we transform our points in the clip space
  Vector4 projected_point =  MultiplyMatrixVector4(projectionMatrix, Vector4FromVector3(vertex, 1.0f));

  // Perform perspective division
  if (projected_point.w != 0.0f)
  {
      projected_point.x /= projected_point.w;
      projected_point.y /= projected_point.w;
      projected_point.z /= projected_point.w;
  }

  /*
  // Check if the point is within the view frustum
  if (projected_point.x < -1.0f || projected_point.x > 1.0f ||
      projected_point.y < -1.0f || projected_point.y > 1.0f ||
      projected_point.z < -1.0f || projected_point.z > 1.0f)
  {
      return (Vector2){-1, -1};  // Return an invalid point
  }
  */

  // Map to screen space
  float x_screen = (projected_point.x + 1.0f) * 0.5f * GetScreenWidth();
  float y_screen = (1.0f - projected_point.y) * 0.5f * GetScreenHeight();
    
  // this point is in the screen space
  return (Vector2){x_screen, y_screen};
}
// #endregion

// #region Model
#define MAX_VERTICES 10000
#define MAX_FACES 10000

typedef struct {
    Vector3 vertices[MAX_VERTICES];
    int vertexCount;
    int faces[MAX_FACES][3];
    int faceCount;
    Matrix transform;
} Model3D;

Model3D loadOBJ(const char* filename)
{
    Model3D model = {0};
    model.transform = MatrixIdentity();
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return model;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            Vector3 vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            model.vertices[model.vertexCount++] = vertex;
        } else if (line[0] == 'f' && line[1] == ' ') {
            int v1, v2, v3;
            sscanf(line, "f %d %d %d", &v1, &v2, &v3);
            model.faces[model.faceCount][0] = v1 - 1;
            model.faces[model.faceCount][1] = v2 - 1;
            model.faces[model.faceCount][2] = v3 - 1;
            model.faceCount++;
        }
    }

    fclose(file);
    return model;
}

void drawModel( Model3D* model, const myCam* cam)
{
    Matrix viewMatrix = myGetCameraViewMatrix(cam);
    for (int i = 0; i < model->faceCount; i++)
    {
      // Apply model transformation before view transformation
      Vector3 v1 = MultiplyMatrixVector(model->transform, model->vertices[model->faces[i][0]]);
      Vector3 v2 = MultiplyMatrixVector(model->transform, model->vertices[model->faces[i][1]]);
      Vector3 v3 = MultiplyMatrixVector(model->transform, model->vertices[model->faces[i][2]]);

      // Apply view transformation
      v1 = MultiplyMatrixVector(viewMatrix, v1);
      v2 = MultiplyMatrixVector(viewMatrix, v2);
      v3 = MultiplyMatrixVector(viewMatrix, v3);

      Vector2 p1 = perspective_projection(v1, cam);
      Vector2 p2 = perspective_projection(v2, cam);
      Vector2 p3 = perspective_projection(v3, cam);

      if (p1.x >= 0 && p2.x >= 0 && p3.x >= 0)
      {
          DrawTriangleLines(p1, p2, p3, BLACK);
      }
    }
}
// #endregion

// #region Main
int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "3D");
    SetTargetFPS(60);

    Model3D model = loadOBJ("./cube.OBJ");

    myCam cam = {
      .position = (Vector3){0, 0, 10},
      .lookAtPoint = (Vector3){0, 0, 0},
      .upVector = (Vector3){0, 1, 0},
      .aspect = (float)screenWidth / (float)screenHeight,
      .fov = 60.0f,
      .near = 1,
      .far = 100
    };

    while (!WindowShouldClose())
    {
      // #region CameraMovement
      if (IsKeyDown(KEY_A)) cam.position.x += 0.1f;
      if (IsKeyDown(KEY_D)) cam.position.x -= 0.1f;
      if (IsKeyDown(KEY_W)) cam.position.y -= 0.1f;
      if (IsKeyDown(KEY_S)) cam.position.y += 0.1f;
      if (IsKeyDown(KEY_Q)) cam.position.z -= 0.1f;
      if (IsKeyDown(KEY_E)) cam.position.z += 0.1f;
      // #endregion

      Matrix viewMatrix = myGetCameraViewMatrix(&cam);

      // #region Drawing
      BeginDrawing();
      ClearBackground(WHITE);

      drawModel(&model, &cam);

      EndDrawing();
      // #endregion
    }
    CloseWindow();
}
// #endregion