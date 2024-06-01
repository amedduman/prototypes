#include "defer.h"
#include "raylib.h"
#include "rlgl.h" // Required for: Vertex definition
#include "raymath.h"

#define MAX_POINTS 12

const int screenWidht = 400;
const int screenHeight = 400;

Vector2 RotatePoint(Vector2 center, Vector2 point, float angle)
{
    float cos = cosf(angle);
    float sin = sinf(angle);
    float x = point.x - center.x;
    float y = point.y - center.y;
    point = (Vector2){x * cos - y * sin, x * sin + y * cos};
    point = Vector2Add(center, point);
    return point;
}

int main()
{
    InitWindow(screenWidht, screenHeight, "Tetris???");
    SetTargetFPS(24);

    Vector2 center = {200, 200};
    float rad = (2 * PI) / (float)MAX_POINTS;
    Vector2 positions[MAX_POINTS] = {0, 0};
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
        if (IsKeyDown(KEY_RIGHT))
        {
            for (int i = 0; i < MAX_POINTS; i++)
            {
                positions[i] = RotatePoint(center, positions[i], PI / 16);
            }
        }

        if (IsKeyDown(KEY_LEFT))
        {
            for (int i = 0; i < MAX_POINTS; i++)
            {
                positions[i] = RotatePoint(center, positions[i], -PI / 16);
            }
        }

        defer(BeginDrawing(), EndDrawing())
        {
            ClearBackground(DARKGRAY);

            DrawCircleV(center, 5, MAROON);
            for (int i = 0; i < MAX_POINTS; i++)
            {
                DrawCircleV(positions[i], 5, WHITE);
            }

            DrawLineStrip(&positions[0], MAX_POINTS, SKYBLUE);
            DrawLineV(positions[0], positions[MAX_POINTS - 1], SKYBLUE);
        }
    }

    CloseWindow();
}