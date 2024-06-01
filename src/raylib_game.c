#include <raylib.h>
#include "dino.h"
#include "defer.h"

const int screenWidht = 400;
const int screenHeight = 400;

int main()
{
    InitWindow(screenWidht, screenHeight, "Tetris???");
    SetTargetFPS(60);

    Dino dino;
    InitDino(&dino);

    Rectangle r = {300,300, 100,100};

    while (!WindowShouldClose())
    {
        Update(&dino);

        defer(BeginDrawing(), EndDrawing())
        {
            ClearBackground(DARKGRAY);

            DrawRectangleLinesEx(r, 5, MAROON);
            Draw(&dino);
            bool isColliding = CheckCollisionRecs(dino.rect, r);
            DrawRect(&dino, isColliding);
        }   
    }
    
    FreeDinoRes(&dino);
    CloseWindow();
}