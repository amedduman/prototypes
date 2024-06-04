#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "vec.h"
#include "Particle.h"

const int screenWidht = 400;
const int screenHeight = 400;

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        Particle *particles = vector_create();

        for (int i = 0; i < 100; i++)
        {
            Particle p;
            particle_create(&p, (Vector2){GetRandomValue(0, 400),GetRandomValue(0, 400)}, (Vector2){GetRandomValue(-3, 3), GetRandomValue(-3,3)});
            vector_add(&particles, p);
        }

        while (!WindowShouldClose())
        {
            for (int i = 0; i < vector_size(particles); i++)
            {
                particle_update(&particles[i]);
            }

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                for (int i = 0; i < vector_size(particles); i++)
                {

                    particle_draw(&particles[i]);
                }
            }
        }
    }
}