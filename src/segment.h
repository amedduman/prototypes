#pragma once 
#include "raylib.h"
#include <raymath.h>

typedef struct
{
    Vector2 start;
    Vector2 end;
    float length;
    float angleInRad;
} segment;

Vector2 segment_calc_end(Vector2 start, float length, float angleInRad)
{
    return (Vector2){
        .x = start.x + length * cosf(angleInRad),
        .y = start.y + length * sinf(angleInRad)
    };
}

segment segment_create(Vector2 start, float length, float angleInRad)
{
    return (segment){
        .start = start,
        .end = segment_calc_end(start, length, angleInRad),
        .length = length,
        .angleInRad = angleInRad,
        };
}

void segment_draw(segment* segment)
{
    DrawLineEx(segment->start, segment->end, 3, WHITE);
}