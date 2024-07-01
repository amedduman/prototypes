#include "include/raylib.h"
#include "src/matrix.h"
#include <stdio.h>

typedef struct
{
  Vector2 points[5];
} square;

void square_init(square* s, Vector2 pos, float width, float height)
{
  s->points[0] = pos;
  s->points[1] = (Vector2){pos.x - width / 2, pos.y - height / 2}; // top-left corner
  s->points[2] = (Vector2){pos.x + width / 2, pos.y - height / 2}; // top-right corner
  s->points[3] = (Vector2){pos.x - width / 2, pos.y + height / 2}; // bottom-left corner
  s->points[4] = (Vector2){pos.x + width / 2, pos.y + height /2}; // bottom-right corner
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
    square_init(&s, (Vector2){400, 200}, 50, 50);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        square_draw(&s);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}