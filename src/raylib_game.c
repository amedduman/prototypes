#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "vec.h"

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

        Vector2* circles = vector_create();

        while (!WindowShouldClose())
        {
            float cosVal = (cosf(angle) + 1) * 0.5;
            float sinVal = (sinf(angle) + 1) * 0.5;

            pos.x = Lerp(screenWidht - radius, radius, cosVal);
            pos.y = Lerp(screenHeight - radius, radius, sinVal);

            vector_add(&circles, pos);

            angle += PI * 0.01;

            char sinValText[50];
            snprintf(sinValText, 50, "Sin: %.2f", sinVal);

            char cosValText[50];
            snprintf(cosValText, 50, "Cos: %.2f", cosVal);

            

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(DARKGRAY);
                
                for (int i = 0; i < vector_size(circles); i++)
                {
                    DrawCircleV(circles[i], radius, MAROON);
                }
                

                DrawText(cosValText, 100, 110, 10, WHITE);
                DrawText(sinValText, 100, 100, 10, WHITE);
            }
        }
    }
}