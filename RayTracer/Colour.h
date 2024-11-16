#pragma once

#include "RayTracer.h"

#include "Colour.h"
#include <string_view>
#include <stdexcept>
#include <algorithm>
#include <string>


/* An RGB colour */
struct Colour
{
    FLT m_R=1;
    FLT m_G=1;
    FLT m_B=1;

    Colour operator+(const Colour other) const
    {
        return Colour{ m_R + other.m_R, m_G + other.m_G, m_B + other.m_B };
    }
    void operator+=(const Colour other)
    {
        m_R += other.m_R;
        m_G += other.m_G;
        m_B += other.m_B;
    }

    Colour operator*(FLT other) const
    {
        return Colour{ m_R * other, m_G * other, m_B * other };
    }

    void operator*=(FLT other)
    {
        m_R *= other;
        m_G *= other;
        m_B *= other;
    }

    /*Colour operator/(FLT other) const
    {
        return Colour{ m_R / other, m_G / other, m_B / other };
    }*/

    static Colour fromHex(std::string_view hex)
    {
        if (hex.length() != 6 && hex.length() != 7)
            throw std::runtime_error("Hex length must be 6 or 7");
        if (hex.length() == 7)
            hex = hex.substr(1, 6);

        const auto result = std::ranges::find_if_not(hex, [](char ch)
        {
            auto isHex = [](char ch)
                {
                    if (ch >= '0' && ch <= '9')
                        return true;
                    if (ch >= 'A' && ch <= 'F')
                        return true;
                    if (ch >= 'a' && ch <= 'f')
                        return true;
                    return false;
                }(ch);
            return isHex;
        });
        if (result != hex.end())
            throw std::runtime_error("Invalid cahracter in hex");

        size_t pos;
        const auto r = std::stoi(std::string(hex.substr(0, 2)), &pos, 16) / 255.f;
        const auto g = std::stoi(std::string(hex.substr(2, 2)), &pos, 16) / 255.f;
        const auto b = std::stoi(std::string(hex.substr(4, 2)), &pos, 16) / 255.f;

        return Colour{ r, g, b };
    }
};
