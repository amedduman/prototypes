#include "defer.h"
#include "utils.h"
#include <raylib.h>
#include <raymath.h>

int main()
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow())
    {
        while (!WindowShouldClose())
        {
            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

            }
        }
    }
}