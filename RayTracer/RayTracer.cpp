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
#include <chrono>
#include <iostream>

void doit()
{
    const auto camera = Vec(0, 0, -1);
    //auto plainRed = [](Point _) { return Colour{ 1,0,0 }; };
    auto plainBlue = [](Point _) { static const auto colour = Colour::fromHex("#0000FF"); return colour; };
    auto plainPink = [](Point _) { static const auto colour = Colour::fromHex("#803980"); return colour; };
    //Material which has a chessboard pattern
    auto checker = [](Point position)
        {
            static const auto m_Colour1 = Colour::fromHex("#420500");
            static const auto m_Colour2 = Colour::fromHex("#e6b87d");
            if (int((position.m_x + 5.0) * 3.0) % 2 == int(position.m_z * 3.0) % 2)
                return m_Colour1;
            return m_Colour2;
        };

    //auto redSphere = Sphere{ { 0,0,0 }, 0.5, { plainRed } };
    auto blueSphere = Sphere{ {  0.75f, -0.1f, 1.0f  }, 0.6f, { plainBlue } };
    auto pinkSphere = Sphere{ { -0.75f, -0.1f, 2.25f }, 0.6f, { plainPink } };
    auto ground = Sphere{ { 0, 10000.5, 1 }, 10000.0, { checker, 0.2f, 1, 1, 0.2f } };


    std::vector<Sphere> objects
    {
        ground,
        blueSphere,
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
        2*960, // width
        2*540 // height
#endif
    };
    const auto begin = std::chrono::steady_clock::now();

    const auto image = RenderEngine::render(scene);

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;

    image.writePPM("img.ppm");
}

int main()
{
	doit();
	return 0;
}
