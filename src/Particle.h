#pragma once
#include <raylib.h>
#include <raymath.h>

typedef struct Particle
{
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
}Particle;

void particle_create(Particle* particle, Vector2 pos, Vector2 vel, Vector2 acc)
{
    particle->pos = pos;
    particle->vel = vel;
    particle->acc = acc;
}

void particle_update(Particle* particle)
{
    particle->vel = Vector2Add(particle->vel, particle->acc);
    particle->pos = Vector2Add(particle->pos, particle->vel);
}

void particle_draw(Particle* particle)
{
    DrawCircleV(particle->pos, 5, WHITE);
}