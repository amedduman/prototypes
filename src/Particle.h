#pragma once
#include <raylib.h>
#include <raymath.h>

typedef struct
{
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
    float radius;
    float mass;
} particle;

void particle_create(particle *particle, Vector2 pos, Vector2 vel, Vector2 acc, float radius, float mass)
{
    particle->pos = pos;
    particle->vel = vel;
    particle->acc = acc;
    particle->radius = radius;
    particle->mass = mass;
}

// returns angle (in radians) between two particles
float particle_angle_to(particle *this, particle *other)
{
    float dy = other->pos.y - this->pos.y;
    float dx = other->pos.x - this->pos.x;
    return atan2(dy, dx);
}

float particle_distance_to(particle *this, particle *other)
{
    float dx = other->pos.x - this->pos.x;
    float dy = other->pos.y - this->pos.y;

    return sqrtf(dx * dx + dy * dy);
}

// this will gravitate towards other
void particle_gravitate_to(particle *this, particle *other)
{
    float dist = particle_distance_to(other, this);
    float magnitude_of_gravity = other->mass / dist * dist;
    Vector2 gravity = Vector2Subtract(other->pos, this->pos);
    gravity = Vector2Normalize(gravity);
    this->acc = (Vector2){gravity.x * magnitude_of_gravity, gravity.y * magnitude_of_gravity};
}

void particle_update(particle *particle)
{
    particle->vel = Vector2Add(particle->vel, particle->acc);
    particle->pos = Vector2Add(particle->pos, particle->vel);
}

void particle_draw(particle *particle)
{
    DrawCircleV(particle->pos, particle->radius, WHITE);
}