#pragma once
#include "raylib.h"
#include "vec.h"

Vector2 RotatePoint(Vector2 center, Vector2 point, float angle)
{
    float cos = cosf(angle);
    float sin = sinf(angle);
    float x = point.x - center.x;
    float y = point.y - center.y;
    point = (Vector2){x * cos - y * sin, x * sin + y * cos};
    point = Vector2Add(center, point);
    return point;
}

typedef struct ship
{
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
    ;
    Vector2 *trianglePoints;
} ship;

void ship_create(ship *ship)
{
    ship->pos = (Vector2){200, 200};
    ship->vel = (Vector2){0, 0};
    ship->acc = (Vector2){0, 0};

    Vector2 point1 = {ship->pos.x + 20, ship->pos.y + 20};
    Vector2 point2 = {ship->pos.x, ship->pos.y - 20};
    Vector2 point3 = {ship->pos.x - 20, ship->pos.y + 20};

    ship->trianglePoints = vector_create();

    vector_add(&ship->trianglePoints, point1);
    vector_add(&ship->trianglePoints, point2);
    vector_add(&ship->trianglePoints, point3);
}

void ship_rotate(ship *ship, bool do_clock_wise)
{
    for (int i = 0; i < vector_size(ship->trianglePoints); i++)
    {
        if (do_clock_wise)
        {
            ship->trianglePoints[i] = RotatePoint(ship->pos, ship->trianglePoints[i], PI * 0.5 * GetFrameTime());
        }
        else
        {
            ship->trianglePoints[i] = RotatePoint(ship->pos, ship->trianglePoints[i], PI * -0.5 * GetFrameTime());
        }
    }
}

void ship_update(ship* ship)
{
    ship->vel = Vector2Add(ship->vel, ship->acc);
    ship->pos = Vector2Add(ship->pos, ship->vel);

    for (int i = 0; i < vector_size(ship->trianglePoints); i++)
    {
        ship->trianglePoints[i] = Vector2Add(ship->trianglePoints[i], ship->vel);
    }    
}

void ship_draw(ship* ship)
{
    DrawTriangle(
        ship->trianglePoints[0],
        ship->trianglePoints[1],
        ship->trianglePoints[2],
        WHITE);
}

float GetRandomFloat(float min, float max)
{
    // Generate a random integer using GetRandomValue
    int randomInt = GetRandomValue(0, 10000);

    // Scale the random integer to the desired float range
    float randomFloat = ((float)randomInt / 10000.0f) * (max - min) + min;

    return randomFloat;
}