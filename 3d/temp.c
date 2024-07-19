#include "include/raylib.h"
#include "include/raymath.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

  // Check if the point is within the view frustum
  if (projected_point.x < -1.0f || projected_point.x > 1.0f ||
      projected_point.y < -1.0f || projected_point.y > 1.0f ||
      projected_point.z < -1.0f || projected_point.z > 1.0f)
  {
      return (Vector2){-1, -1};  // Return an invalid point
  }

  // Map to screen space
    float x_screen = (projected_point.x + 1.0f) * 0.5f * GetScreenWidth();
    float y_screen = (1.0f - projected_point.y) * 0.5f * GetScreenHeight();
    
    // this point is in the screen space
    return (Vector2){x_screen, y_screen};
}

// REMEMBER -> the incoming point is in the camera space
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

#define MAX_VERTICES 10000
#define MAX_FACES 10000

typedef struct {
    Vector3 vertices[MAX_VERTICES];
    int vertexCount;
    int faces[MAX_FACES][3];
    int faceCount;
} Model3D;

Model3D loadOBJ(const char* filename) {
    Model3D model = {0};
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

void drawModel(const Model3D* model, const myCam* cam) {
    Matrix viewMatrix = myGetCameraViewMatrix(cam);
    for (int i = 0; i < model->faceCount; i++) {
        Vector3 v1 = MultiplyMatrixVector(viewMatrix, model->vertices[model->faces[i][0]]);
        Vector3 v2 = MultiplyMatrixVector(viewMatrix, model->vertices[model->faces[i][1]]);
        Vector3 v3 = MultiplyMatrixVector(viewMatrix, model->vertices[model->faces[i][2]]);

        Vector2 p1 = perspective_projection(v1, cam);
        Vector2 p2 = perspective_projection(v2, cam);
        Vector2 p3 = perspective_projection(v3, cam);

        if (p1.x >= 0 && p2.x >= 0 && p3.x >= 0) {
            DrawTriangleLines(p1, p2, p3, BLACK);
        }
    }
}

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "3D");
    Model3D model = loadOBJ("./cube.OBJ");
    // check if model is loaded 
    if (model.vertexCount == 0 || model.faceCount == 0)
    {
      printf("Failed to load model\n");
      return 1;
    }

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
 
      BeginDrawing();
      ClearBackground(WHITE);

      drawModel(&model, &cam);

      EndDrawing();
    }
    CloseWindow();
    return 0;
}

/*
https://chatgpt.com/c/8d1d208b-5b02-4476-852d-6ec89bd66884

Clipping space is a crucial concept in the computer graphics pipeline. It refers to a coordinate system where geometric primitives are clipped to the view frustum before being projected onto the screen. Here's a detailed breakdown of its role and the transformations involved:
1. Graphics Pipeline Overview

The graphics pipeline is a sequence of steps used to transform 3D coordinates into a 2D image on your screen. Key stages include:

    Modeling Transformation: Converts model coordinates to world coordinates.
    View Transformation: Converts world coordinates to camera/view coordinates.
    Projection Transformation: Converts view coordinates to clip coordinates.
    Clipping: Clips primitives to the view frustum.
    Viewport Transformation: Maps normalized device coordinates to screen coordinates.

2. Clipping Space

Clipping space is an intermediate coordinate system used in the graphics pipeline. It's a result of the projection transformation, where coordinates are transformed from view space to clip space.
Transformations to Clipping Space:

    Modeling Transformation:
        vworld=Mmodel⋅vmodelvworld​=Mmodel​⋅vmodel​
        Transforms vertices from model coordinates to world coordinates using the model matrix (MmodelMmodel​).

    View Transformation:
        vview=Mview⋅vworldvview​=Mview​⋅vworld​
        Transforms vertices from world coordinates to view coordinates using the view matrix (MviewMview​).

    Projection Transformation:
        vclip=Mproj⋅vviewvclip​=Mproj​⋅vview​
        Transforms vertices from view coordinates to clip coordinates using the projection matrix (MprojMproj​).

3. Role of Clipping Space

In clip space, coordinates are represented in a 4D homogeneous form (x,y,z,w)(x,y,z,w). The main role of clipping space is to prepare the primitives for clipping against the view frustum. The view frustum is typically defined by the following volume in clip space:

    −w≤x≤w−w≤x≤w
    −w≤y≤w−w≤y≤w
    −w≤z≤w−w≤z≤w

Primitives outside this volume are clipped (partially or fully removed) to ensure they are within the viewable area.
4. Clipping Process

The clipping process involves checking and potentially modifying the vertices of each primitive to ensure they lie within the view frustum. This is essential to avoid rendering artifacts and to optimize rendering by not processing primitives outside the view.
5. Transformations After Clipping Space

After clipping, the coordinates are transformed from clip space to normalized device coordinates (NDC) and then to screen coordinates:

    Normalized Device Coordinates (NDC):
        Divide the xx, yy, and zz components by the ww component:
            xndc=xclipwclipxndc​=wclip​xclip​​
            yndc=yclipwclipyndc​=wclip​yclip​​
            zndc=zclipwclipzndc​=wclip​zclip​​
        This step transforms clip coordinates to a normalized cube from [−1,1][−1,1] in all three dimensions.

    Viewport Transformation:
        Maps NDC coordinates to screen coordinates based on the viewport dimensions.

Summary

Clipping space is used in the projection transformation stage of the graphics pipeline, preparing primitives for clipping against the view frustum. It ensures that only visible parts of the scene are processed in subsequent stages, optimizing rendering performance and accuracy.

If you have any specific aspects or further details you want to delve into, feel free to ask!
*/
