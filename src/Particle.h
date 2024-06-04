#pragma once
#include <raylib.h>
#include <raymath.h>

typedef struct Particle
{
    Vector2 pos;
    Vector2 vel;
}Particle;

void particle_create(Particle* particle, Vector2 pos, Vector2 vel)
{
    particle->pos = pos;
    particle->vel = vel;
}

void particle_update(Particle* particle)
{
    particle->pos = Vector2Add(particle->pos, particle->vel);
}

void particle_draw(Particle* particle)
{
    DrawCircleV(particle->pos, 10, WHITE);
}