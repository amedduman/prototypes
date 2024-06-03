#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "vec.h"

const int screenWidht = 400;
const int screenHeight = 400;

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        Vector2 center = {screenWidht * 0.5, screenHeight * 0.4};
        float radius = 5;
        float x_angle = 0;
        float y_angle = 0;

        Vector2* circles = vector_create();

        while (!WindowShouldClose())
        {
            center.x = center.x + cosf(x_angle) * 5;
            center.y = center.y + sinf(y_angle) * 3;

            vector_add(&circles, center);

            x_angle += PI * 0.01;
            y_angle += PI * 0.03;

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(DARKGRAY);
                
                for (int i = 0; i < vector_size(circles); i++)
                {
                    DrawCircleV(circles[i], radius, PINK);
                }
            }
        }
    }
}