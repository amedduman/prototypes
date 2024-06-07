#include "defer.h"
#include "utils.h"
#include "particle.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

const int screenWidth = 400;
const int screenHeight = 400;

void spring(particle* p0, particle* p1, float springLength, float springConst)
{
    float springForceMag = (Vector2Distance(p0->pos, p1->pos) - springLength) * springConst;
    Vector2 springForce = Vector2Subtract(p0->pos, p1->pos);
    springForce = ut_Vector2MulVal(springForce, springForceMag);
    
    p1->acc = springForce;
    p0->acc = ut_Vector2MulVal(springForce, -1.0);
}

int main()
{
    defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        Vector2 gravity = {0.0f, 9.0f};

        particle particleA;
        particle_create(&particleA, (Vector2){300, 100}, (Vector2){3, 0}, (Vector2){0, 0}, 10, 1, 0.9f);
        particleA.col = RED;
        particle particleB;
        particle_create(&particleB, (Vector2){100, 100}, (Vector2){0, 0}, (Vector2){0, 0}, 10, 1, 0.9f);
        particle particleC;
        particle_create(&particleC, (Vector2){200, 200}, (Vector2){0, 0}, (Vector2){0, 0}, 10, 1, 0.9f);
        particleC.col = BLUE;
        
        float springConst = 0.01f;
        float springLength = 50;

        while (!WindowShouldClose())
        {
            float x = 0;
            float y = 0;
            if(IsKeyDown(KEY_RIGHT))
            {
                x = 40;
            }
            else if(IsKeyDown(KEY_LEFT))
            {
                x = -40;
            }
            if(IsKeyDown(KEY_UP))
            {
                y = -40;
            }
            else if(IsKeyDown(KEY_DOWN))
            {
                y = 40;
            }
            particleA.vel = (Vector2){x, y};

            spring(&particleA, &particleB, springLength, springConst);
            spring(&particleB, &particleC, springLength, springConst);
            spring(&particleC, &particleA, springLength, springConst);

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