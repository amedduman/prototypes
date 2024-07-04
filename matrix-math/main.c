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

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game");

    square s;
    square_init(&s, (Vector2){0, 0}, 50, 50);

    my_matrix trMatrix;
    my_matrix_init(&trMatrix, 3, 3);
    float tx = 50;
    float ty = 100;
    float trMatrixValues[9] = {1, 0, tx, 0, 1, ty, 0, 0, 1};
    trMatrix.values = trMatrixValues;

    my_matrix scaleMatrix;
    my_matrix_init(&scaleMatrix, 2, 2);
    float sx = 2;
    float sy = 2;
    float scaleMatrixValues[4] = {sx, 0, 0, sy};
    scaleMatrix.values = scaleMatrixValues;

    my_matrix rotMatrix;
    my_matrix_init(&rotMatrix, 2, 2);
    float angleInRad = PI / 8;
    float rotMatrixValues[4] = {cosf(angleInRad), -sinf(angleInRad), sinf(angleInRad), cosf(angleInRad)};
    rotMatrix.values = rotMatrixValues;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
      if(IsKeyPressed(KEY_SPACE))
      {
        for (int i = 0; i < 5; i++)
        {
          s.points[i] = my_matrix_mul_with_Vector2(&trMatrix, s.points[i]);
        }
      }

      BeginDrawing();

      ClearBackground(RAYWHITE);

      square_draw(&s);

      EndDrawing();
    }
    CloseWindow();
    return 0;
}