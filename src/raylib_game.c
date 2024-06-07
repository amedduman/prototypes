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

        Vector2 gravity = {0.0f,0.0f};
        float friction = 0.99f;

        particle particleA;
        particle_create(&particleA, (Vector2){300, 100}, (Vector2){3, 0}, (Vector2){0, 0}, 10, 1, friction);
        particleA.col = RED;
        particle particleB;
        particle_create(&particleB, (Vector2){100, 100}, (Vector2){0, 0}, (Vector2){0, 0}, 10, 1, friction);
        particle particleC;
        particle_create(&particleC, (Vector2){200, 200}, (Vector2){0, 0}, (Vector2){0, 0}, 10, 1, friction);
        particleC.col = BLUE;
        
        float springConst = 0.02f;
        float springLength = 50;

        bool is_particleA_hold = false;

        while (!WindowShouldClose())
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
               if(ut_IsPointCircleIntersect(GetMousePosition(), particleA.pos, particleA.radius))
                {
                    particleA.col = GREEN;
                    is_particleA_hold = true;
                } 
            }
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                particleA.col = RED;
                is_particleA_hold = false;
            }
            if(is_particleA_hold)
            {
                particleA.pos = GetMousePosition();
            }
            
            particle_spring_to(&particleA, &particleB, springLength, springConst);
            particle_spring_to(&particleB, &particleC, springLength, springConst);
            particle_spring_to(&particleC, &particleA, springLength, springConst);

            particleA.acc = Vector2Add(particleA.acc, gravity);
            particleB.acc = Vector2Add(particleB.acc, gravity);
            particleC.acc = Vector2Add(particleC.acc, gravity);

            particle_update(&particleA);
            particle_update(&particleB);
            particle_update(&particleC);

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                DrawLineEx(particleA.pos, particleB.pos, 3, DARKBLUE);
                DrawLineEx(particleB.pos, particleC.pos, 3, DARKBLUE);
                DrawLineEx(particleC.pos, particleA.pos, 3, DARKBLUE);

                particle_draw(&particleA);
                particle_draw(&particleB);
                particle_draw(&particleC);
            }
        }
    }
}