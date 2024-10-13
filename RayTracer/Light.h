#pragma once

#include "RayTracer.h"

#include "Point.h"
#include "Colour.h"
#include <string_view>

// A light, described as a position and a colour (no intensity (yet))
struct Light
{
    Point m_P;
    Colour m_C;

    Light(Point p, std::string_view colour)
        : m_P(p), m_C(Colour::fromHex(colour))
    {
    }
};
