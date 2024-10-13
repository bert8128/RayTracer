#pragma once

#include "RayTracer.h"

#include "Colour.h"
#include "Point.h"
#include <string_view>
#include <functional>

//Material has a colour function (based on position) and properties which tells us how it reacts to light
struct Material
{
    using ColourFunc = std::function<Colour(Point position)>;

    FLT m_Ambient = 0.05f;
    FLT m_Diffuse = 1.f;
    FLT m_Specular = 1.f;
    FLT m_Reflection = 0.5f;
    ColourFunc m_Func;

    Material(ColourFunc func)
        : m_Func(func)
    {
    }

    Material(ColourFunc func, FLT ambient, FLT diffuse, FLT specular, FLT reflection)
        : m_Func(func), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_Reflection(reflection)
    {
    }

    Colour colourAt(Point position) const
    {
        return m_Func(position);
    }
};
