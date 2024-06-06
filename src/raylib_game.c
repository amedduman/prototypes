#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "vec.h"
#include "Particle.h"
#include "ship.h"

const int screenWidht = 400;
const int screenHeight = 400;

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);
        ship player_ship;
        ship_create(&player_ship);

        while (!WindowShouldClose())
        {
            if (IsKeyDown(KEY_A))
            {
                ship_rotate(&player_ship, false);
            }
            if (IsKeyDown(KEY_D))
            {
                ship_rotate(&player_ship, true);
            }
            if (IsKeyDown(KEY_W))
            {
                player_ship.accMag = 0.1;
            }
            else
            {
                player_ship.accMag = 0;
                player_ship.vel = (Vector2){0,0};
            }

            ship_update(&player_ship);

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                ship_draw(&player_ship);
            }
        }
    }
}