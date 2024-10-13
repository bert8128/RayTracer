#pragma once

#include "RayTracer.h"

#include "Colour.h"
#include "Image.h"
#include "Point.h"
#include "Ray.h"
#include "Scene.h"

#include <limits>
#include <cmath>

//Renders 3D objects into 2D objects using ray tracing

struct RenderEngine
{
    static Image render(const Scene& scene)
    {
        const auto width = scene.m_W;
        const auto height = scene.m_H;
        const auto aspectRatio = static_cast<FLT>(width) / height;
        const FLT x0 = -1.f;
        const FLT x1 = +1.f;
        const auto xstep = (x1 - x0) / (width - 1);
        const auto y0 = -1.f / aspectRatio;
        const auto y1 = +1.f / aspectRatio;
        const auto ystep = (y1 - y0) / (height - 1);

        const auto camera = scene.m_Camera;
        Image image(width, height);

        for (auto j = 0; j < height; ++j)
        {
            auto y = y0 + j * ystep;
            for (auto i = 0; i < width; ++i)
            {
                auto x = x0 + i * xstep;
                const auto ray = Ray(camera, Point{ x, y, 0.f } - camera);
                image.setPixel(i, j, RenderEngine::rayTrace(ray, scene));
            }
        }
        return image;
    }

    static Colour rayTrace(Ray ray, const Scene& scene, int depth=0)
    {
        constexpr auto MAX_DEPTH = 3;
        constexpr FLT MIN_DISPLACE = 0.0001f;
            
        auto sphereDist = RenderEngine::findNearest(ray, scene);
        if (!sphereDist.first)
            return Colour{ 0, 0, 0 };
        const auto hit_pos = ray.m_Origin + ray.m_Direction * sphereDist.second;
        const auto hit_normal = sphereDist.first->normal(hit_pos);
        auto colour = RenderEngine::colourAt(*sphereDist.first, hit_pos, hit_normal, scene);
        if (depth < MAX_DEPTH)
        {
            auto new_ray_pos = hit_pos + hit_normal * MIN_DISPLACE;
            auto new_ray_dir = ray.m_Direction - hit_normal * 2 * ray.m_Direction.dotProd(hit_normal);
            auto new_ray = Ray(new_ray_pos, new_ray_dir);
            auto nextColour = RenderEngine::rayTrace(new_ray, scene, depth + 1) * sphereDist.first->m_Material.m_Reflection;
            colour += nextColour;
        }
        return colour;
    }

    static std::pair<const Sphere*, FLT> findNearest(Ray ray, const Scene& scene)
    {
        FLT distMin = std::numeric_limits<FLT>::max();
        const Sphere* pSphere = nullptr;
        for (auto& sphere : scene.m_Objects)
        {
            auto dist = sphere.intersects(ray);
            if (dist)
            {
                if (nullptr == pSphere || *dist < distMin)
                {
                    pSphere = &sphere;
                    distMin = *dist;
                }
            }
        }
        return { pSphere, distMin };
    }

    template <typename F1, typename F2>
    static FLT mmax(F1 f1, F2 f2)
    {
        if (f1 > f2)
            return static_cast<FLT>(f1);
        return static_cast<FLT>(f2);
    }
    static Colour colourAt(const Sphere& sphere, Point hit_pos, Vec normal, const Scene& scene)
    {
        const auto& material = sphere.m_Material;
        const auto objColour = material.colourAt(hit_pos);
        const auto hpToCam = scene.m_Camera - hit_pos;
        const FLT specularK = 50;
        Colour colour = Colour::fromHex("#FFFFFF");
        colour *= material.m_Ambient;
        for (auto& light : scene.m_Lights)
        {
            const auto hpToLight = Ray(hit_pos, light.m_P - hit_pos);
            //Lambert
            colour += objColour * material.m_Diffuse * mmax(normal.dotProd(hpToLight.m_Direction), 0);
            //Blinn-Phong
            auto half_vector = (hpToLight.m_Direction + hpToCam).normalise();
            colour += light.m_C * material.m_Specular * pow((mmax(normal.dotProd(half_vector), 0)), specularK);
        }
        return colour;
    }
};




