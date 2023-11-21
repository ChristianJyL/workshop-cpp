#include <sil/sil.hpp>
#include <iostream>
#include "random.hpp"
#include <math.h>

void allGreen(sil::Image image)
{
    for (glm::vec3 &color : image.pixels())
    {
        color.r = 0.f;
        color.b = 0.f;
    }
    image.save("output/exercice1.png");
}

void inverseCanals(sil::Image image)
{
    for (glm::vec3 &color : image.pixels())
    {
        std::swap(color.r, color.b);
    }
    image.save("output/exercice2.png");
}

void blackAndWhite(sil::Image image)
{
    for (glm::vec3 &color : image.pixels())
    {
        float average = (color.r + color.g + color.b) / 3.f;
        color.r = average;
        color.g = average;
        color.b = average;
    }
    image.save("output/exercice3.png");
}

void negatif(sil::Image image)
{
    for (glm::vec3 &color : image.pixels())
    {
        color.r = 1 - color.r;
        color.g = 1 - color.g;
        color.b = 1 - color.b;
    }
    image.save("output/exercice4.png");
}

void degrade()
{
    sil::Image image{300 /*width*/, 200 /*height*/};
    // On passe sur tous les x et tous les y, et on accède au pixel correspondant :
    for (int x{0}; x < image.width(); x++)
    {
        float value = static_cast<float>(x) / image.width();
        for (int y{0}; y < image.height(); y++)
        {
            image.pixel(x, y).r = value;
            image.pixel(x, y).b = value;
            image.pixel(x, y).g = value;
        }
    }
    image.save("output/exercice5.png");
}

void mirror(sil::Image image)
{
    for (int x{0}; x < image.width() / 2; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            std::swap(image.pixel(x, y), image.pixel(image.width() - 1 - x, y));
        }
    }
    image.save("output/exercice6.png");
}

void noise(sil::Image image)
{
    for (glm::vec3 &color : image.pixels())
    {
        int random_value = random_int(1, 5);
        if (random_value == 1)
        {
            color.r = random_float(0.f, 1.f);
            color.g = random_float(0.f, 1.f);
            color.b = random_float(0.f, 1.f);
        }
    }
    image.save("output/exercice7.png");
}

void rotate(sil::Image image)
{
    sil::Image rotateImage{image.height(), image.width()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            rotateImage.pixel(y, x) = image.pixel(x, image.height() - y - 1);
        }
    }
    rotateImage.save("output/exercice8.png");
}

void RGBSplit(sil::Image image)
{
    sil::Image newImage{image};
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if (x + 30 <= image.width() - 1)
            {
                newImage.pixel(x, y).b = image.pixel(x + 30, y).b;
            }
            else
            {
                newImage.pixel(x, y).b = image.pixel(image.width()-1, y).b;
            }
            if (x - 30 >= 0)
            {
                newImage.pixel(x, y).r = image.pixel(x - 30, y).r;
            }
            else
            {
                newImage.pixel(x, y).r = image.pixel(0, y).r;
            }
        }
    }
    newImage.save("output/exercice9.png");
}


void luminosity(sil::Image image, bool sombre)
{
    float puissance {0};
    if (sombre){
        puissance = 2;
    }
    else{
        puissance = 0.5;
    }
    for (glm::vec3 &color : image.pixels()){
        color.r = std::pow(color.r, puissance);
        color.b = std::pow(color.b, puissance);
        color.g = std::pow(color.g, puissance);
    }
    image.save("output/exercice10.png");

}

int main()
{
    sil::Image image{"images/logo.png"};
    sil::Image photo{"images/photo.jpg"};
    allGreen(image);
    inverseCanals(image);
    blackAndWhite(image);
    negatif(image);
    degrade();
    mirror(image);
    noise(image);
    rotate(image);
    RGBSplit(image);
    luminosity(photo, false);
}