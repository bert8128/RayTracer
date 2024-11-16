#include "BMP.h"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

void writeBmp(
    const std::vector<std::vector<uint8_t>> bgr,
    uint32_t width,
    uint32_t height,
    std::ostream& out)
{
    constexpr uint32_t BYTES_PER_PIXEL = 3;
    constexpr uint32_t FILE_HEADER_SIZE = 14;
    constexpr uint32_t INFO_HEADER_SIZE = 40;

    auto write16 = [&out](uint16_t value)
        {
            out.put(static_cast<uint8_t>(value));
            out.put(static_cast<uint8_t>(value >> 8));
        };
    auto write32 = [&out](uint32_t value)
        {
            out.put(static_cast<uint8_t>(value));
            out.put(static_cast<uint8_t>(value >> 8));
            out.put(static_cast<uint8_t>(value >> 16));
            out.put(static_cast<uint8_t>(value >> 24));
        };

    // Ensure no overflow. This is conservative but simple.
    if (width >= 30000 || height >= 30000)
    {
        std::cout << "Image too big\n";
        assert(width <= 30000 && height <= 30000);
        return;
    }

    const uint32_t input_stride = width * BYTES_PER_PIXEL;
    const uint32_t output_stride = ((input_stride + 3) / 4) * 4;
    const uint32_t padding = output_stride - input_stride;
    const uint32_t image_size = output_stride * height;
    const uint32_t file_size = image_size + FILE_HEADER_SIZE + INFO_HEADER_SIZE;

    // Signature.
    out.put('B');
    out.put('M');
    // Image file size.
    write32(file_size);
    // Reserved.
    write16(0);
    write16(0);
    // Offset to image data.
    write32(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    // BITMAPINFOHEADER
    write32(INFO_HEADER_SIZE);
    write32(width);
    write32(height);
    // Planes.
    write16(1);
    // Bits per pixel.
    write16(BYTES_PER_PIXEL * 8);
    // Compression.
    write32(0);
    // Image data size.
    write32(image_size);
    // X/Y pixels per meter, for printing. This value is 72 DPI.
    write32(2835);
    write32(2835);
    // Colours in colour table.
    write32(0);
    // Important colours in colour table.
    write32(0);

    // Write the image data.
    if (height != bgr.size())
    {
        assert(height == bgr.size());
        std::cout << "height != bgr.size()\n";
        return;
    }

    //for (size_t r = 0; r < bgr.size(); ++r)
    for (uint32_t y1 = height; y1 > 0; --y1)
    {
        auto& row = bgr[y1-1];

        if (width*3 != row.size())
        {
            assert(width*3 == row.size());
            std::cout << "width != row.size()\n";
            return;
        }

        //for (size_t r = 0; r < bgr.size(); ++r)
        {
            //out.write(reinterpret_cast<const char*>(&bgr[(y1 - 1) * input_stride]), input_stride);
            out.write(reinterpret_cast<const char*>(row.data()), input_stride);
            for (uint32_t x = 0; x < padding; ++x)
                out.put(0);
        }
    }
}

void writeBmp(
    const std::vector<std::vector<uint8_t>>& bgr,
    const uint32_t width,
    const uint32_t height,
    const char* filename)
{
    std::ofstream out(filename, std::ios::binary);
    writeBmp(bgr, width, height, out);
}
/*
int main()
{
    const unsigned height = 300;
    const unsigned width = 400;
    std::vector< uint8_t> image(width * height * 3, 0);
    //uint8_t image[width * height * 3] = { 0 };
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            uint8_t* pixel = &image[(y * width + x) * 3];
            pixel[2] = static_cast<uint8_t>(x); // red
            pixel[1] = static_cast<uint8_t>(y); // green
            pixel[0] = static_cast<uint8_t>(x + y); // blue
        }
    }

    std::ofstream out("out.bmp", std::ios::binary);
    writeBmp(image, width, height, out);
}
*/
