#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

extern void writeBmp(
    const std::vector<std::vector<uint8_t>>& bgr,
    uint32_t width,
    uint32_t height,
    const char* filename);
