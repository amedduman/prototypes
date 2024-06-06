#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "vec.h"
#include "particle.h"
#include "ship.h"

const int screenWidht = 800;
const int screenHeight = 800;

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        particle sun;
        particle planet;

        sun.pos = (Vector2){400, 400};
        sun.mass = 3;
        sun.radius = 20;

        planet.pos = (Vector2){250, 100};
        planet.radius = 5;
        planet.vel = (Vector2){40, 0};

        while (!WindowShouldClose())
        {
            if (IsKeyDown(KEY_A))
            {
                particle_gravitate_to(&planet, &sun);
                particle_update(&planet);
            }

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                particle_draw(&sun);
                particle_draw(&planet);

                DrawLineEx(planet.pos, Vector2Add(planet.vel, planet.pos), 3, BLACK);
            }
        }
    }
}