#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

const int screenWidht = 400;
const int screenHeight = 400;

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Game"), CloseWindow())
    {
        SetTargetFPS(24);

        Vector2 pos = {screenWidht * 0.5, 0};
        float radius = 30;
        float angle = 0;

        while (!WindowShouldClose())
        {
            float sinVal = (sinf(angle) + 1) * 0.5;
            pos.y = Lerp(screenHeight - radius, radius, sinVal);

            angle += PI * 0.01;

            char angleText[50];
            snprintf(angleText, 50, "Sin: %.2f", sinVal);

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(DARKGRAY);
                DrawCircleV(pos, radius, MAROON);
                DrawText(angleText, 100, 100, 10, WHITE);
            }
        }
    }
}