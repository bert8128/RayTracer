#pragma once

#include "RayTracer.h"

#include "Point.h"
#include "Vec.h"

// A ray, from a point in a direction
struct Ray
{
    Point m_Origin;
    Vec m_Direction;

    Ray(Point origin, Vec direction)
        : m_Origin(origin), m_Direction(direction.normalise())
    {
    }
};
