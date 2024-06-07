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
    springForce = (Vector2){springForce.x * springForceMag, springForce.y * springForceMag};

    p1->acc = springForce;
    p0->acc = (Vector2){springForce.x * -1, springForce.y * -1};
}

int main()
{
    defer(InitWindow(screenWidth, screenHeight, "Math"), CloseWindow())
    {
        SetTargetFPS(60);

        Vector2 gravity = {0.0f, 3.0f};

        //Vector2 posA = {GetRandomValue(0, screenWidth), GetRandomValue(0, screenHeight)};
        particle particleA;
        particle_create(&particleA, (Vector2){300, 100}, (Vector2){3, 0}, (Vector2){0, 0}, 10, 1, 0.9f);
        particleA.col = RED;
        //Vector2 posB = {GetRandomValue(0, screenWidth), GetRandomValue(0, screenHeight)};
        particle particleB;
        particle_create(&particleB, (Vector2){100, 100}, (Vector2){0, 0}, (Vector2){0, 0}, 10, 1, 0.9f);
        
        float springConst = 0.01f;
        float springLength = 50;

        while (!WindowShouldClose())
        {
            /* Vector2 springOrigin = GetMousePosition();
            float springForceMag = (Vector2Distance(particleA.pos, springOrigin) - springLength) * springConst;
            Vector2 springForce = Vector2Subtract(springOrigin, particleA.pos);
            springForce = Vector2Normalize(springForce); // why normalization?
            springForce = (Vector2){springForce.x * springForceMag, springForce.y * springForceMag};
            particleA.acc = Vector2Add(springForce, gravity); */

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
            //particleB.vel = (Vector2){-x, -y};

            spring(&particleA, &particleB, springLength, springConst);

            particle_update(&particleA);
            particle_update(&particleB);

            defer(BeginDrawing(), EndDrawing())
            {
                ClearBackground(GOLD);

                DrawLineEx(particleA.pos, particleB.pos, 3, DARKBLUE);

                particle_draw(&particleA);
                particle_draw(&particleB);
            }
        }
    }
}