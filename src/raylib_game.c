#include "defer.h"
#include "raylib.h"
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

Vector2 RotatePoint_PreCalcCosSin(Vector2 center, Vector2 point, float cos, float sin)
{
    float x = point.x - center.x;
    float y = point.y - center.y;
    point = (Vector2){x * cos - y * sin, x * sin + y * cos};
    point = Vector2Add(center, point);
    return point;
}

void Rotate(Vector2 *positions, Vector2 center)
{
    float angle = PI / 16;
    float cosCW = cosf(angle);
    float sinCW = sinf(angle);
    float cosCCW = cosf(-angle);
    float sinCCW = sinf(-angle);

    if (IsKeyDown(KEY_D))
    {
        for (int i = 0; i < MAX_POINTS; i++)
        {
            positions[i] = RotatePoint_PreCalcCosSin(center, positions[i], cosCW, sinCW);
        }
    }

    if (IsKeyDown(KEY_A))
    {
        for (int i = 0; i < MAX_POINTS; i++)
        {
            positions[i] = RotatePoint_PreCalcCosSin(center, positions[i], cosCCW, sinCCW);
        }
    }
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
        Rotate(positions, center);

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