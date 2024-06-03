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
            // * (sinf(angle) < 0 ? 1 : -1)
            //pos.y = sinf(angle) * screenHeight;
            
            pos.y = Lerp(screenHeight * 0.5, radius, sinf(angle));
            angle += PI * 0.01;
            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(DARKGRAY);
                DrawCircleV(pos, radius, MAROON);
                char angleText[50];
                snprintf(angleText, 50, "Sin: %.2f", sinf(angle));
                DrawText(angleText, 100,100, 10, WHITE);
            }
        }
    }
}