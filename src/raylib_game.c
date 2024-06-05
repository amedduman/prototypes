#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "vec.h"
#include "Particle.h"

const int screenWidht = 400;
const int screenHeight = 400;

float GetRandomFloat(float min, float max)
{
    // Generate a random integer using GetRandomValue
    int randomInt = GetRandomValue(0, 10000);

    // Scale the random integer to the desired float range
    float randomFloat = ((float)randomInt / 10000.0f) * (max - min) + min;

    return randomFloat;
}

int main()
{
    defer(InitWindow(screenWidht, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        Vector2 gravity = {0, 0.2};
        Particle *particles = vector_create();

        for (int i = 0; i < 100; i++)
        {
            Particle p;
            particle_create(&p, (Vector2){200, 200}, (Vector2){GetRandomFloat(-5, 5), GetRandomFloat(-5, 5)}, gravity);
            vector_add(&particles, p);
        }

        while (!WindowShouldClose())
        {
            if (IsKeyDown(KEY_A))
            {
                for (int i = 0; i < vector_size(particles); i++)
                {
                    particle_update(&particles[i]);
                }
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