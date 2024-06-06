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
    float accMag;
    Vector2 front;
    Vector2 *trianglePoints;
} ship;

void ship_create(ship *ship)
{
    ship->pos = (Vector2){200, 200};
    ship->vel = (Vector2){0, 0};
    ship->accMag = 0;
    ship->front = (Vector2){ship->pos.x, ship->pos.y - 20};

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
    float angle = PI * 0.5 * GetFrameTime();
    // ship->dir = Vector2Add(ship->dir, ship->pos);

    for (int i = 0; i < vector_size(ship->trianglePoints); i++)
    {
        if (do_clock_wise)
        {
            ship->trianglePoints[i] = RotatePoint(ship->pos, ship->trianglePoints[i], angle);
        }
        else
        {
            ship->trianglePoints[i] = RotatePoint(ship->pos, ship->trianglePoints[i], -angle);
        }
    }

    if (do_clock_wise)
    {
        ship->front = RotatePoint(ship->pos, ship->front, angle);
    }
    else
    {
        ship->front = RotatePoint(ship->pos, ship->front, -angle);
    }
}

void ship_update(ship *ship)
{
    if (ship->accMag > 0)
    {
        Vector2 ship_real_dir = Vector2Subtract(ship->front, ship->pos);
        ship_real_dir = Vector2Normalize(ship_real_dir);
        ship->vel = Vector2Add(ship->vel, ship_real_dir);
        ship->pos = Vector2Add(ship->pos, ship->vel);

        for (int i = 0; i < vector_size(ship->trianglePoints); i++)
        {
            ship->trianglePoints[i] = Vector2Add(ship->trianglePoints[i], ship->vel);
        }

        ship->front = Vector2Add(ship->front, ship->vel);
    }
}

void ship_draw(ship *ship)
{
    DrawTriangle(
        ship->trianglePoints[0],
        ship->trianglePoints[1],
        ship->trianglePoints[2],
        WHITE);

    DrawLineEx(ship->pos, ship->front, 5, RED);

    //DrawText(TextFormat("%f, %f", ship->dir.x, ship->dir.y), 100, 100, 15, WHITE);
    // DrawCircleV(Vector2Add(ship->pos, ship->dir), 10, RED);
}

float GetRandomFloat(float min, float max)
{
    // Generate a random integer using GetRandomValue
    int randomInt = GetRandomValue(0, 10000);

    // Scale the random integer to the desired float range
    float randomFloat = ((float)randomInt / 10000.0f) * (max - min) + min;

    return randomFloat;
}