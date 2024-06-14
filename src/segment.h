#pragma once 
#include "raylib.h"
#include <raymath.h>

struct segment
{
    Vector2 start;
    Vector2 end;
    float length;
    float angleInRad;
    struct segment* parent;
    Color col;
};

Vector2 segment_calc_end(Vector2 start, float length, float angleInRad)
{
    return (Vector2){
        .x = start.x + length * cosf(angleInRad),
        .y = start.y + length * sinf(angleInRad)
    };
}

struct segment segment_create(Vector2 start, float length, float angleInRad, Color col, struct segment* parent)
{
    return (struct segment){
        .start = start,
        .end = segment_calc_end(start, length, angleInRad),
        .length = length,
        .angleInRad = angleInRad,
        .parent = parent,
        .col = col,
        };
}

void segment_draw(struct segment* segment)
{
    DrawLineEx(segment->start, segment->end, 3, segment->col);
}