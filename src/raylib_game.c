#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "vec.h"
#include "Particle.h"

const int screenWidht = 400;
const int screenHeight = 400;

float GetRandomFloat(float min, float max)
{
    // Generate a random integer using GetRandomValue
    int randomInt = GetRandomValue(0, 10000);

    // Scale the random integer to the desired float range
    float randomFloat = ((float)randomInt / 10000.0f) * (max - min) + min;

    return randomFloat;
}

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
    defer(InitWindow(screenWidht, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        Vector2 center = {200, 200};
        Vector2 point1 = {center.x + 20, center.y + 20};
        Vector2 point2 = {center.x, center.y - 20};
        Vector2 point3 = {center.x - 20, center.y + 20};

        Vector2 *trianglePoints = vector_create();

        vector_add(&trianglePoints, point1);
        vector_add(&trianglePoints, point2);
        vector_add(&trianglePoints, point3);

        while (!WindowShouldClose())
        {
            if (IsKeyDown(KEY_A))
            {
                for (int i = 0; i < vector_size(trianglePoints); i++)
                {
                    trianglePoints[i] = RotatePoint(center, trianglePoints[i], PI * -0.5 * GetFrameTime());
                }
            }
            if (IsKeyDown(KEY_D))
            {
                for (int i = 0; i < vector_size(trianglePoints); i++)
                {
                    trianglePoints[i] = RotatePoint(center, trianglePoints[i], PI * 0.5 * GetFrameTime());
                }
            }

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);
                DrawTriangle(trianglePoints[0], trianglePoints[1], trianglePoints[2], WHITE);
            }
        }
    }
}