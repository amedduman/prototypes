#include "defer.h"
#include "utils.h"
#include "particle.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

const int screenWidth = 400;
const int screenHeight = 400;

int main()
{
    defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        particle weight;
        particle_create(&weight, (Vector2){100,100}, (Vector2){0,0}, (Vector2){10,0}, 10, 1, 0.4f);
        float springConst = 0.2;
        while (!WindowShouldClose())
        {
            Vector2 springOrigin = GetMousePosition();
            float springForceMag = Vector2Distance(weight.pos, springOrigin) * springConst;
            Vector2 springForce = Vector2Subtract(springOrigin, weight.pos);
            springForce = Vector2Normalize(springForce);
            springForce = (Vector2){springForce.x * springForceMag, springForce.y * springForceMag};
            //weight.acc = springForce;

            particle_update(&weight);

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);
                
                particle_draw(&weight);
                DrawCircleV(springOrigin, 2, MAROON);
            }
        }
    }
}