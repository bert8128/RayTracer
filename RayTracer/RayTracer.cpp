#include "RayTracer.h"

#include "Vec.h"
#include "Colour.h"
#include "Image.h"
#include "Light.h"
#include "Point.h"
#include "Material.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "RenderEngine.h"
#include "BMP.h"

#include <chrono>
#include <iostream>

void doit()
{
    const auto camera = Vec(0, 0, -1);
    //auto plainRed = [](Point) { return Colour{ 1,0,0 }; };
    auto plainBlue = [](Point) { static const auto colour = Colour::fromHex("#0000FF"); return colour; };
    auto plainPink = [](Point) { static const auto colour = Colour::fromHex("#803980"); return colour; };
    //Material which has a chessboard pattern
    auto checker = [](Point position)
        {
            static const auto m_Colour1 = Colour::fromHex("#420500");
            static const auto m_Colour2 = Colour::fromHex("#e6b87d");
            if (int((position.m_x + 5.0) * 3.0) % 2 == int(position.m_z * 3.0) % 2)
                return m_Colour1;
            return m_Colour2;
        };
    const auto blueMaterial    = Material{ plainBlue, 0.2f, 0.9, 0.9, 0.1f };
    const auto pinkMaterial    = Material{ plainPink, 0.2f, 1, 1, 0.2f };
    const auto checkerMaterial = Material{ checker,   0.2f, 1, 1, 0.5f };

    //auto redSphere = Sphere{ { 0,0,0 }, 0.5, { plainRed } };
    const auto blueSphere =       Sphere{ {  0.75f, -0.2f, 1.0f  }, 0.6f,    blueMaterial };
    const auto secondBlueSphere = Sphere{ {  0.25f, -1.0f, 5.0f  }, 0.6f,    blueMaterial };
    const auto pinkSphere =       Sphere{ { -0.75f, -0.1f, 2.25f }, 0.6f,    pinkMaterial };
    const auto ground =           Sphere{ {  0, 10000.5,   1     }, 10000.0, checkerMaterial };

    std::vector<Sphere> objects
    {
        ground,
        blueSphere,
        secondBlueSphere,
        pinkSphere
    };
    std::vector<Light> lights
    {
        Light{ {  1.5,  -0.5, -10.0 }, "#FFFFFF" },
        Light{ { -0.5, -10.5,   0.0 }, "#E6E6E6" }
    };
    Scene scene
    {
        camera,
        objects,
        lights,
#ifdef _DEBUG
        320, // width
        200 // height
#else
        5*960, // width
        5*540 // height
#endif
    };
    const auto image = [&scene]()
    {
        const auto begin = std::chrono::steady_clock::now();

        const auto image = RenderEngine::render(scene);

        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;

        return image;
    }();

    /* {
        const auto begin = std::chrono::steady_clock::now();
        image.writePPM("img.ppm");
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;
    }*/

    {
        auto toByte = [](FLT d) { return std::clamp(static_cast<int>(d * 255), 0, 255); };

        FLT m00 = 0;
        for (size_t j = 0; j < image.m_PixelRows.size(); ++j)
        {
            auto& r = image.m_PixelRows[j];
            for (size_t i = 0; i < r.size(); ++i)
            {
                const auto& pixel = r[i];
                const auto m = std::max(pixel.m_R, std::max(pixel.m_G, pixel.m_B));
                if (m > m00)
                    m00 = m;
            }
        }
        const auto m = 1.0+((m00-1.0)/2.0);// > 1 ? m00 : (FLT)1;// std::min((FLT)1, m00);
        std::vector<std::vector<uint8_t>> bmp(image.m_PixelRows.size());
        {
            const auto begin = std::chrono::steady_clock::now();
            for (size_t j = 0; j < image.m_PixelRows.size(); ++j)
            {
                auto& r = image.m_PixelRows[j];
                auto& out = bmp[j];
                out.resize(r.size() * 3);
                for (size_t i = 0; i < r.size(); ++i)
                {
                    const auto& pixel = r[i];
                    //const auto m0 = std::max(pixel.m_R, std::max(pixel.m_G, pixel.m_B));
                    //const auto m = std::min((FLT)1, m0);
                    out[i * 3 + 0] = static_cast<uint8_t>(toByte(pixel.m_B/m));
                    out[i * 3 + 1] = static_cast<uint8_t>(toByte(pixel.m_G/m));
                    out[i * 3 + 2] = static_cast<uint8_t>(toByte(pixel.m_R/m));
                }
            }
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;
        }
        {
            const auto begin = std::chrono::steady_clock::now();
            writeBmp(bmp, image.m_W, image.m_H, "img1.bmp");
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;
        }
    }
}

int main()
{
	doit();
	return 0;
}

