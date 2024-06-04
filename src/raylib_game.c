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
        Texture2D arrow = LoadTexture("./src/resources/arrow.png"); 

        Rectangle sourceRect = {0,0,arrow.width,arrow.height};
        Rectangle destRect = {200, 200, (float)arrow.width * 0.2, (float)arrow.height * 0.2};
        Vector2 origin = {(float)destRect.width / 2, (float)destRect.height / 2};

        while (!WindowShouldClose())
        {
            Vector2 mouse = GetMousePosition();
            Vector2 arrowPos = (Vector2){destRect.x, destRect.y};
            float dy = mouse.y - arrowPos.y;
            float dx = mouse.x - arrowPos.x;
            float angleInRad = atan2f(dy, dx);

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                DrawTexturePro(arrow, sourceRect, destRect, origin, RAD2DEG * angleInRad, WHITE);
                DrawText(TextFormat("%f", RAD2DEG * angleInRad), 100, 100, 10, BLACK);
                DrawCircleV(GetMousePosition(), 5, MAROON);
                DrawLineV(arrowPos, mouse, RED);
            }
        }

        UnloadTexture(arrow);
    }
}