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
        arrow.width = 100;
        arrow.height = 100;

        Rectangle sourceRect = {0,0,100,100};
        Rectangle destRect = {200, 200, 100,100};
        Vector2 origin = {0, 50};

        int rotation = 0;

        while (!WindowShouldClose())
        {

            rotation++;

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(DARKGRAY);

                DrawTexturePro(arrow, sourceRect, destRect, origin, (float)rotation, WHITE);
                //DrawTexture(arrow, 150, 150, WHITE);                
            }
        }

    }
}