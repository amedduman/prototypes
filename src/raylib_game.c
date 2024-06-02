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

void Rotate(Vector2 *points, Vector2 center)
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
            points[i] = RotatePoint_PreCalcCosSin(center, points[i], cosCW, sinCW);
        }
    }

    if (IsKeyDown(KEY_A))
    {
        for (int i = 0; i < MAX_POINTS; i++)
        {
            points[i] = RotatePoint_PreCalcCosSin(center, points[i], cosCCW, sinCCW);
        }
    }
}

void Scale(Vector2 *points, Vector2 center)
{
    float scaleFactor = 1;

    if (IsKeyDown(KEY_W))
        scaleFactor = 1.1;
    else if (IsKeyDown(KEY_S))
        scaleFactor = 0.9;
    else 
        return;
    for (int i = 0; i < MAX_POINTS; i++)
    {
        Vector2 p = points[i];
        Vector2 centerToP = Vector2Subtract(p, center);
        Vector2 scaledP = Vector2Scale(centerToP, scaleFactor);
        Vector2 newP = Vector2Add(scaledP, center);
        points[i] = newP;
        //points[i] = Vector2Scale(points[i], scaleFactor);
        //points[i] = Vector2Add(points[i], center);
    }
}

void Translate(Vector2 *points, Vector2 velocity)
{
    for (int i = 0; i < MAX_POINTS; i++)
    {
        points[i] = Vector2Add(points[i], velocity);
    }
}

int main()
{
    InitWindow(screenWidht, screenHeight, "Tetris???");
    SetTargetFPS(24);

    Vector2 center = {200, 200};
    float rad = (2 * PI) / (float)MAX_POINTS;
    Vector2 points[MAX_POINTS] = {0, 0};
    for (int i = 0; i < MAX_POINTS; i++)
    {
        Vector2 p2 = {0, 0};

        p2 = (Vector2){cosf(i * rad), sinf(i * rad)};

        if (i % 2 == 0)
            p2 = Vector2Scale(p2, 100);
        else
            p2 = Vector2Scale(p2, 50);
        p2 = Vector2Add(center, p2);

        points[i] = p2;
    }

    while (!WindowShouldClose())
    {
        Vector2 velocity = {0, 0};
        if (IsKeyDown(KEY_RIGHT))
            velocity.x += 1;
        else if (IsKeyDown(KEY_LEFT))
            velocity.x -= 1;
        if (IsKeyDown(KEY_UP))
            velocity.y -= 1;
        else if (IsKeyDown(KEY_DOWN))
            velocity.y += 1;
        velocity = Vector2Normalize(velocity);

        Scale(points, center);
        Rotate(points, center);
        Translate(points, velocity);
        center = Vector2Add(center, velocity);

        defer(BeginDrawing(), EndDrawing())
        {
            ClearBackground(DARKGRAY);

            DrawCircleV(center, 5, MAROON);
            for (int i = 0; i < MAX_POINTS; i++)
            {
                DrawCircleV(points[i], 5, WHITE);
            }

            DrawLineStrip(&points[0], MAX_POINTS, SKYBLUE);
            DrawLineV(points[0], points[MAX_POINTS - 1], SKYBLUE);
        }
    }

    CloseWindow();
}