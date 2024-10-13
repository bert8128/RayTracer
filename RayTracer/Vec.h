#pragma once

#include "RayTracer.h"

#include <cmath>

/* A 3D mathematical vector */
struct Vec
{
    FLT m_x;
    FLT m_y;
    FLT m_z;

    Vec(FLT x, FLT y, FLT z) : m_x(x), m_y(y), m_z(z) {}

    Vec operator+(const Vec other) const
    {
        return Vec{ m_x + other.m_x, m_y + other.m_y, m_z + other.m_z };
    }

    Vec operator-(const Vec other) const
    {
        return Vec{ m_x - other.m_x, m_y - other.m_y, m_z - other.m_z };
    }

    Vec operator*(FLT other) const
    {
        return Vec{ m_x * other, m_y * other, m_z * other };
    }

    Vec operator/(FLT other) const
    {
        return Vec{ m_x / other, m_y / other, m_z / other };
    }
        
    FLT dotProd(const Vec other) const
    {
        return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
    }

    FLT magnitude() const
    {
        return sqrt(dotProd(*this));
    }

    Vec normalise() const
    {
        return *this / magnitude();
    }
};
