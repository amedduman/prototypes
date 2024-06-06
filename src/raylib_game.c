#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include "vec.h"
#include "particle.h"

const int screenWidht = 800;
const int screenHeight = 800;

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        while (!WindowShouldClose())
        {

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);
            }
        }
    }
}