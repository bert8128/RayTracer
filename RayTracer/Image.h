#pragma once

#include "RayTracer.h"

#include "Colour.h"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <format>

// An image
struct Image
{
    int m_W;
    int m_H;
    std::vector< std::vector<Colour>> m_PixelRows;

    Image(int w, int h)
        : m_W(w), m_H(h)
    {
        m_PixelRows.resize(m_H);
        for (auto& r : m_PixelRows)
            r.resize(m_W);
    }

    void setPixel(int x, int y, Colour c)
    {
        m_PixelRows[y][x] = c;
    }

    void writePPM(std::string f) const
    {
        auto toByte = [](FLT d) { return std::clamp(static_cast<int>(d * 255), 0, 255); };

        std::ofstream outputFile(f.data());

        if (!outputFile.is_open())
            throw std::runtime_error("Couldn't open output file");

        outputFile << std::format("P3 {} {}\n255\n", m_W, m_H);

        for (auto& r : m_PixelRows)
        {
            for (auto c : r)
            {
                outputFile << std::format("{} {} {} ", toByte(c.m_R), toByte(c.m_G), toByte(c.m_B));
            }
        }

        outputFile << "\n";
    }

};
