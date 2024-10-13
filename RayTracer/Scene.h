#pragma once

#include "RayTracer.h"

#include "Point.h"
#include "Sphere.h"

#include <vector>

//Scene has all the information needed for the ray tracing engine
struct Scene
{
    Point m_Camera;
    std::vector<Sphere> m_Objects;
    std::vector<Light> m_Lights;
    int m_W;
    int m_H;
};
