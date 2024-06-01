#include "defer.h"
#include "raylib.h"
#include "rlgl.h" // Required for: Vertex definition
#include "raymath.h"

#define MAX_POINTS 12

const int screenWidht = 400;
const int screenHeight = 400;

int main()
{
    InitWindow(screenWidht, screenHeight, "Tetris???");
    SetTargetFPS(4);

    Vector2 center = {200, 200};
    float rad = (2 * PI) / (float)MAX_POINTS;
    Vector2 positions[10] = {0, 0};
    for (int i = 0; i < MAX_POINTS; i++)
    {
        Vector2 p2 = {0, 0};

        p2 = (Vector2){cosf(i * rad), sinf(i * rad)};

        if (i % 2 == 0)
            p2 = Vector2Scale(p2, 100);
        else
            p2 = Vector2Scale(p2, 50);
        p2 = Vector2Add(center, p2);

        positions[i] = p2;
    }

    while (!WindowShouldClose())
    {
        defer(BeginDrawing(), EndDrawing())
        {
            ClearBackground(DARKGRAY);

            DrawCircleV(center, 5, MAROON);
            for (int i = 0; i < MAX_POINTS; i++)
            {
                DrawCircleV(positions[i], 5, WHITE);
            }

            DrawLineStrip(&positions, MAX_POINTS, SKYBLUE);
            DrawLineV(positions[0], positions[MAX_POINTS-1], SKYBLUE);
        }
    }

    CloseWindow();
}