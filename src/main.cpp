#include <sil/sil.hpp>
#include <iostream>
#include "random.hpp"
#include <math.h>

void allGreen(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        color.r = 0.f;
        color.b = 0.f;
    }
}

void inverseCanals(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        std::swap(color.r, color.b);
    }
}

void blackAndWhite(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        float const average = (color.r + color.g + color.b) / 3.f;
        color = glm::vec3{average};
    }
}

void negatif(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        color = 1.f - color;
    }
}

[[nodiscard]] sil::Image degrade()
{
    sil::Image image{300 /*width*/, 200 /*height*/};
    for (int x{0}; x < image.width(); x++)
    {
        float const value = static_cast<float>(x) / (image.width() - 1);
        for (int y{0}; y < image.height(); y++)
        {
            image.pixel(x,y) = glm::vec3(value);
        }
    }
    return image;
}

void mirror(sil::Image &image)
{
    for (int x{0}; x < image.width() / 2; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            std::swap(image.pixel(x, y), image.pixel(image.width() - 1 - x, y));
        }
    }
}

void noise(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        float random_value = random_float(0.f, 1.f);
        if (random_value < 0.33)
        {
            color = glm::vec3(random_float(0.f, 1.f),random_float(0.f, 1.f),random_float(0.f, 1.f));
        }
    }
}

[[nodiscard]] sil::Image rotate(sil::Image const &image)
{
    sil::Image rotateImage{image.height(), image.width()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            rotateImage.pixel(y, x) = image.pixel(x, image.height() - y - 1);
        }
    }
    return rotateImage;
}

[[nodiscard]] sil::Image RGBSplit(sil::Image const &image)
{
    sil::Image newImage{image};
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            int final_x_forward;
            int final_x_backward;
            if (x + 30 <= image.width() - 1)
            {
                final_x_forward = x + 30;
            }
            else
            {
                final_x_forward = image.width() - 1;
            }
            if (x - 30 >= 0)
            {
                final_x_backward = x - 30;
            }
            else
            {
                final_x_backward = 0;
            }

            // int const final_x = x + 30 <= image.width() - 1
            //                         ? x + 30
            //                         : image.width() - 1;
            newImage.pixel(x, y).b = image.pixel(final_x_backward, y).r;
            newImage.pixel(x, y).r = image.pixel(final_x_forward, y).r;
        }
    }
    return newImage;
}

void luminosity(sil::Image &image, bool sombre)
{
    float puissance{0};
    if (sombre)
    {
        puissance = 2;
    }
    else
    {
        puissance = 0.5;
    }
    for (glm::vec3 &color : image.pixels())
    {
        color = glm::pow(color, glm::vec3{puissance});
    }
}

[[nodiscard]] sil::Image disk()
{
    sil::Image image{500 /*width*/, 500 /*height*/};
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if (std::pow(x - (image.width()/2),2)+ std::pow(y - image.height()/2,2) < 10000)
            {
                image.pixel(x, y) = glm::vec3(1);
            }
        }
    }
    return image;
}

[[nodiscard]] sil::Image circle(float thickess)
{
    sil::Image image{500,500};
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if ((std::pow(x - (image.width()/2),2)+ std::pow(y - image.height()/2,2) < 10000 + thickess) && (std::pow(x - (image.width()/2),2)+ std::pow(y - image.height()/2,2) > 10000 - thickess ))
            {
                image.pixel(x, y) = glm::vec3(1);
            }
        }
    }
    return image;
}

int main()
{
    sil::Image image{"images/logo.png"};
    sil::Image photo{"images/photo.jpg"};

    {
        sil::Image copy{image};
        allGreen(copy);
        copy.save("output/exercice1.png");
    }

    {
        sil::Image copy{image};
        inverseCanals(copy);
        copy.save("output/exercice2.png");
    }

    {
        sil::Image copy{image};
        blackAndWhite(copy);
        copy.save("output/exercice3.png");
    }

    {
        sil::Image copy{image};
        negatif(copy);
        copy.save("output/exercice4.png");
    }
    {
        sil::Image degradeImage{degrade()};
        degradeImage.save("output/exercice5.png");
    }
    {
        sil::Image copy{image};
        mirror(copy);
        copy.save("output/exercice6.png");
    }
    {
        sil::Image copy{image};
        noise(copy);
        copy.save("output/exercice7.png");
    }
    {
        sil::Image rotateImage {rotate(image)};
        rotateImage.save("output/exercice8.png");
    }
    {
        sil::Image rgbImage{RGBSplit(image)};
        rgbImage.save("output/exercice9.png");
    }
    {
        sil::Image copy{photo};
        luminosity(copy, false);
        copy.save("output/exercice10.png");
    }
    {
        sil::Image diskImage{disk()};
        diskImage.save("output/exercice11.png");
    }
    {
        sil::Image circleImage{circle(500)};
        circleImage.save("output/exercice12.png");
    }
}