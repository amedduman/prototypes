#pragma once 
#include "raylib.h"
#include <math.h>
#include <raymath.h>
#include <stdlib.h>
#include <sys/_types/_null.h>

struct segment
{
    Vector2 start;
    Vector2 end;
    float length;
    float angleInRad;
    float localAngle;
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

Vector2 segment_calc_endS(struct segment* segment)
{
    return segment_calc_end(segment->start, segment->length, segment->angleInRad);
}

struct segment segment_create(Vector2 start, float length, float angleInRad, Color col, struct segment* parent)
{
    return (struct segment){
        .start = start,
        .end = segment_calc_end(start, length, angleInRad),
        .length = length,
        .angleInRad = angleInRad,
        .localAngle = angleInRad,
        .parent = parent,
        .col = col,
        };
}

void segment_rotate(struct segment* segment, float angleInRad)
{
    segment->localAngle += angleInRad;
}

void segment_update(struct segment* segment)
{
    segment->angleInRad = segment->localAngle;
    if(segment->parent != NULL)
    {
        segment->start = segment->parent->end;
        segment->angleInRad += segment->parent->angleInRad;
    }
    segment->end = segment_calc_endS(segment);
}

void segment_draw(struct segment* segment)
{
    DrawLineEx(segment->start, segment->end, 3, segment->col);
}