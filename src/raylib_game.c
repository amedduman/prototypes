#include "defer.h"
#include "raylib.h"

const int screenWidht = 400;
const int screenHeight = 400;

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Game"), CloseWindow())
    {
        SetTargetFPS(24);

        while (!WindowShouldClose())
        {
            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(DARKGRAY);

            }
        }
    }
}