#include "include/raylib.h"
#include "include/raymath.h"
#include "src/matrix.h"
#include <math.h>
#include <stdio.h>

typedef struct
{
  Vector2 points[5];
} square;

void square_init(square* s, Vector2 pos, float width, float height)
{
  s->points[0] = (Vector2){pos.x, pos.y};
  s->points[1] = (Vector2){pos.x - width / 2, pos.y - height / 2}; // top-left corner
  s->points[2] = (Vector2){pos.x + width / 2, pos.y - height / 2}; // top-right corner
  s->points[3] = (Vector2){pos.x - width / 2, pos.y + height / 2}; // bottom-left corner
  s->points[4] = (Vector2){pos.x + width / 2, pos.y + height / 2}; // bottom-right corner
}

void square_draw(square* s)
{
  DrawLineEx(s->points[1], s->points[2], 1, BLACK);
  DrawLineEx(s->points[2], s->points[4], 1, BLACK);
  DrawLineEx(s->points[4], s->points[3], 1, BLACK);
  DrawLineEx(s->points[3], s->points[1], 1, BLACK);

}
my_matrix getTranslationMatrix(float tx, float ty)
{
    my_matrix trMatrix;
    my_matrix_init(&trMatrix, 3, 3);
    
    trMatrix.values[0] = 1;
    trMatrix.values[1] = 0;
    trMatrix.values[2] = tx;
    trMatrix.values[3] = 0;
    trMatrix.values[4] = 1;
    trMatrix.values[5] = ty;
    trMatrix.values[6] = 0;
    trMatrix.values[7] = 0;
    trMatrix.values[8] = 1;
    
    return trMatrix;
}

my_matrix getScaleMatrix(float sx, float sy)
{
    my_matrix scaleMatrix;
    my_matrix_init(&scaleMatrix, 3, 3);

    /*
    float scaleMatrixValues[9] = {sx, 0, 0,
                                  0, sy, 0,
                                  0, 0, 1};
    */

    scaleMatrix.values[0] = sx;
    scaleMatrix.values[1] = 0;
    scaleMatrix.values[2] = 0;
    scaleMatrix.values[3] = 0;
    scaleMatrix.values[4] = sy;
    scaleMatrix.values[5] = 0;
    scaleMatrix.values[6] = 0;
    scaleMatrix.values[7] = 0;
    scaleMatrix.values[8] = 1;

    return scaleMatrix;
}

my_matrix getRotationMatrix(float angleInRad)
{
    my_matrix rotMatrix;
    my_matrix_init(&rotMatrix, 3, 3);

    /*
    float rotMatrixValues[9] = {cosf(angleInRad), -sinf(angleInRad), 0,
                                sinf(angleInRad), cosf(angleInRad),  0,
                                0,                0,                 1};
    */

    rotMatrix.values[0] = cosf(angleInRad);
    rotMatrix.values[1] = -sinf(angleInRad);
    rotMatrix.values[2] = 0;
    rotMatrix.values[3] = sinf(angleInRad);
    rotMatrix.values[4] = cosf(angleInRad);
    rotMatrix.values[5] = 0;
    rotMatrix.values[6] = 0;
    rotMatrix.values[7] = 0;
    rotMatrix.values[8] = 1;

    return rotMatrix;
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game");

    square s;
    square_init(&s, (Vector2){200, 200}, 50, 50);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
      Vector2 moveVec = {0,0};
      float speed = 5;

      if (IsKeyDown(KEY_UP))
      {
        moveVec.y = -1;
      }
      else if (IsKeyDown(KEY_DOWN))
      {
        moveVec.y = 1;
      }
      if (IsKeyDown(KEY_RIGHT))
      {
        moveVec.x = 1;
      }
      else if (IsKeyDown(KEY_LEFT))
      {
        moveVec.x = -1;
      }

      moveVec = (Vector2){moveVec.x * speed, moveVec.y * speed};

      my_matrix trMatrix = getTranslationMatrix(moveVec.x, moveVec.y);

      for (int i = 0; i < 5; i++)
      {
          s.points[i] = my_matrix_mul_with_Vector2(&trMatrix, s.points[i]);
      }

      float scaleFactor = 1;
      if (IsKeyDown(KEY_W))
      {
        scaleFactor = 1.1;
      }
      else if (IsKeyDown(KEY_S))
      {
        scaleFactor = 0.9;
      }

      my_matrix scaleMatrix = getScaleMatrix(scaleFactor, scaleFactor);

      Vector2 squareCenter = s.points[0];
      float tx = s.points[0].x;
      float ty = s.points[0].y;
      for (int i = 0; i < 5; i++)
      {
        my_matrix moveToOriginMatrix = getTranslationMatrix(-tx, -ty);
        my_matrix moveToPositionMatrix = getTranslationMatrix(tx, ty);
        
        my_matrix resultmatrix = my_matrix_marix_product(&moveToOriginMatrix, &scaleMatrix);
        resultmatrix = my_matrix_marix_product(&resultmatrix, &moveToPositionMatrix);

        s.points[i] = my_matrix_mul_with_Vector2(&moveToOriginMatrix, s.points[i]);
        s.points[i] = my_matrix_mul_with_Vector2(&scaleMatrix, s.points[i]);
        s.points[i] = my_matrix_mul_with_Vector2(&moveToPositionMatrix, s.points[i]);
      }

      BeginDrawing();

      ClearBackground(RAYWHITE);

      square_draw(&s);

      EndDrawing();
    }
    CloseWindow();
    return 0;
}