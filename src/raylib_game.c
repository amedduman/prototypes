#include "defer.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "vec.h"
#include "particle.h"

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

        particle *particles = vector_create();

        for (int i = 0; i < 100; i++)
        {
            particle p;
            particle_create(&p, (Vector2){200, 200}, (Vector2){GetRandomFloat(-3.0, 3.0), GetRandomFloat(-3.0, 3.0)}, (Vector2){0, 0}, 5, 5);
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
                printf("%zu\n", vector_size(particles));
                for (int i = 0; i < vector_size(particles); i++)
                {
                    particle_draw(&particles[i]);
                }
            }

            if (vector_size(particles) > 0)
            {
                for (size_t i = 100; i > 0;)
                {
                    i--;
                    Vector2 pos = particles[i].pos;
                    float radi = particles[i].radius;
                    if (pos.x > 300 + radi || pos.x < 100 - radi || pos.y > 300 + radi || pos.y < 100 - radi)
                    {
                        vector_remove(particles, i);
                    }
                }
            }

            printf("%zu\n", vector_size(particles));
        }

        vector_free(particles);
    }
}