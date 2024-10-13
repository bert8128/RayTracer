#pragma once

#include "RayTracer.h"

#include "Vec.h"
#include "Material.h"
#include "Ray.h"
#include <optional>

// A mathematical sphere
struct Sphere
{
    Point m_Centre;
    FLT m_Radius;
    Material m_Material;

    //Checks if ray intersects this sphere. Returns distance to intersection or None if there is no intersection
    std::optional<FLT> intersects(Ray ray) const
    {
        const auto sToR = ray.m_Origin - m_Centre;
        // a = 1
        const auto b = 2 * ray.m_Direction.dotProd(sToR);
        const auto c = sToR.dotProd(sToR) - (m_Radius * m_Radius);
        const auto discriminant = b * b - 4 /* *a */ * c;

        if (discriminant > 0)
        {
            auto dist = ((-1 * b) - sqrt(discriminant)) / 2;
            if (dist > 0)
                return dist;
        }
        return {};
    }

    //Returns surface normal to the point on sphere's surface
    Vec normal(const Vec& surfacePoint) const
    {
        return (surfacePoint - m_Centre).normalise();
    }
};
