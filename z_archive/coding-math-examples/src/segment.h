#pragma once 
#include "utils.h"

struct segment
{
    Vector2 start;
    Vector2 end;
    float length;
    float angleInRad;
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

struct segment segment_create(Vector2 start, float length, float angleInRad, Color col)
{
    return (struct segment){
        .start = start,
        .end = segment_calc_end(start, length, angleInRad),
        .length = length,
        .angleInRad = angleInRad,
        .col = col,
        };
}

void segment_point_to(struct segment* segment, Vector2 target)
{
    float dy = target.y - segment->start.y;
    float dx = target.x - segment->start.x;
    float targetAngle = atan2f(dy, dx);

    dy = segment->end.y - segment->start.y;
    dx = segment->end.x - segment->start.x;
    float currentAngle = atan2f(dy, dx);

    float angleDifference = targetAngle - currentAngle;

    segment->end = RotatePoint(segment->start, segment->end, angleDifference);
    segment->angleInRad = targetAngle;
}

void segment_follow(struct segment* segment, Vector2 target)
{
  Vector2 dir = Vector2Subtract(segment->start, target);
  dir = Vector2Normalize(dir);
  dir = ut_Vector2MulVal(dir, segment->length);
  dir = Vector2Add(dir, target);

  segment->start = dir;
  segment->end = segment_calc_endS(segment);
  segment_point_to(segment, target);
}

void segment_draw(struct segment* segment)
{
    DrawLineEx(segment->start, segment->end, 2, segment->col);
}