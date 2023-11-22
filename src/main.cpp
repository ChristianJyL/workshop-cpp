#include <sil/sil.hpp>
#include <iostream>
#include "random.hpp"
#include <math.h>
#include <glm/gtx/matrix_transform_2d.hpp>

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
            image.pixel(x, y) = glm::vec3(value);
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
            color = glm::vec3(random_float(0.f, 1.f), random_float(0.f, 1.f), random_float(0.f, 1.f));
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

void disk(sil::Image &image, int rayon)
{
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if (std::pow(x - (image.width() / 2), 2) + std::pow(y - image.height() / 2, 2) < (std::pow(rayon, 2)))
            {
                image.pixel(x, y) = glm::vec3(1);
            }
        }
    }
}

void circle(sil::Image &image, int const &rayon, float const &thickess, int const &center_x, int const &center_y)
{
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if ((std::pow(x - center_x, 2) + std::pow(y - center_y, 2) < std::pow(rayon, 2) + thickess) && (std::pow(x - center_x, 2) + std::pow(y - center_y, 2) > std::pow(rayon, 2) - thickess))
            {
                image.pixel(x, y) = glm::vec3(1);
            }
        }
    }
}

void rosace(sil::Image &image, int const &rayon, int const &thinkness)
{
    int const center_x = image.width() / 2;
    int const center_y = image.height() / 2;
    circle(image, rayon, thinkness, center_x, center_y);
    for (int i{0}; i < 6; i++)
    {
        circle(image, rayon, thinkness, center_x + rayon * std::cos(i * (M_PI / 3)), center_x + rayon * std::sin(i * (M_PI / 3)));
    }
}

[[nodiscard]] sil::Image mosaique(sil::Image const &image, int repetition, bool mirror)
{
    int const mosaique_width = image.width() * repetition;
    int const mosaique_height = image.height() * repetition;
    sil::Image newImage{mosaique_width, mosaique_height};

    for (int x{0}; x < mosaique_width; x++)
    {
        for (int y{0}; y < mosaique_height; y++)
        {

            int const newX = x % image.width();
            int const newY = y % image.height();
            if (mirror && x / image.width() % 2 == 1)
            {
                newImage.pixel(x, y) = image.pixel(image.width() - newX - 1, newY);
            }
            else
            {
                newImage.pixel(x, y) = image.pixel(newX, newY);
            }
        }
    }
    return newImage;
}

/*
old version avec boucle imbriquée donc pas fou fou
[[nodiscard]] sil::Image mosaique(sil::Image const& image)
{
    sil::Image newImage {image.width()*5,image.height()*5};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            for (int i {0}; i<5 ; i++){
                for (int j{0}; j < 5 ; j++){
                    newImage.pixel(x+image.width()*i,y+image.height()*j) = image.pixel(x,y);
                }
            }
        }
    }
    return newImage;
}
}*/

void glitch(sil::Image &image)
{
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float const random_value = random_float(0.f, 1.f);
            if (random_value < 0.001f)
            {
                // std::cout << "glitch à la position " << x << " " << y << std::endl;
                int const random_width = random_int(1, 30);
                int const random_height = random_int(1, 10);
                int const random_x = random_int(1, image.width() - random_width);
                int const random_y = random_int(1, image.height() - random_height);

                for (int i{0}; i < random_width; i++)
                {
                    if (x + i < image.width() && random_x + i < image.width())
                    {
                        std::swap(image.pixel(x + i, y), image.pixel(random_x + i, random_y));
                    }
                    for (int j{0}; j < random_height; j++)
                    {
                        if (y + j < image.height() && random_y + j < image.height() && random_x + i < image.width() && x + i < image.width())
                        {
                            std::swap(image.pixel(x + i, y + j), image.pixel(random_x + i, random_y + j));
                        }
                    }
                }
            }
        }
    }
}

void glitch2(sil::Image &image, int number_of_glitch)
{
    for (int i{0}; i < number_of_glitch; i++)
    {
        int const random_width = random_int(1, 25);
        int const random_height = random_int(1, 10);
        int const random_x1 = random_int(1, image.width() - random_width);
        int const random_y1 = random_int(1, image.height() - random_height);
        int const random_x2 = random_int(1, image.width() - random_width);
        int const random_y2 = random_int(1, image.height() - random_height);

        for (int x{0}; x < random_width; x++)
        {
            for (int y{0}; y < random_height; y++)
            {
                std::swap(image.pixel(random_x1 + x, random_y1 + y), image.pixel(random_x2 + x, random_y2 + y));
            }
        }
    }
}

glm::vec2 rotated(glm::vec2 point, glm::vec2 center_of_rotation, float angle)
{
    return glm::vec2{glm::rotate(glm::mat3{1.f}, angle) * glm::vec3{point - center_of_rotation, 0.f}} + center_of_rotation;
}

[[nodiscard]] sil::Image vortex(sil::Image const&image)
{
    sil::Image newImage{image};
    int const center_x{image.width()/2};
    int const center_y{image.height()/2};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float const angle =glm::distance(glm::vec2(x,y), glm::vec2(center_x, center_y))/10.f;
            glm::vec2 const new_position = rotated(glm::vec2{x, y}, glm::vec2{center_x, center_y}, angle);
            if (new_position.x < image.width() && new_position.y < image.height() && new_position.x > 0 && new_position.y > 0)
            {
                newImage.pixel(x, y) = image.pixel(new_position.x, new_position.y);
            }
            else
            {
                newImage.pixel(x,y) = glm::vec3(0);
            }
        }
    }
    return newImage;
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
        sil::Image rotateImage{rotate(image)};
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
        sil::Image blackImage{500, 500};
        disk(blackImage, 100);
        blackImage.save("output/exercice11.png");
    }
    {
        sil::Image blackImage{500, 500};
        circle(blackImage, 100, 500, blackImage.width() / 2, blackImage.height() / 2);
        blackImage.save("output/exercice12.png");
    }
    {
        sil::Image blackImage{500, 500};
        rosace(blackImage, 100, 500);
        blackImage.save("output/exercice13.png");
    }
    {
        sil::Image mosaiqueImage{mosaique(image, 5, false)};
        mosaiqueImage.save("output/exercice14.png");
    }
    {
        sil::Image mosaiqueImage{mosaique(image, 5, true)};
        mosaiqueImage.save("output/exercice15.png");
    }
    {
        sil::Image copy{image};
        glitch(copy);
        copy.save("output/exercice16.png");
    }
    {
        sil::Image copy{image};
        glitch2(copy, 100);
        copy.save("output/exercice16.png");
    }
    {
        sil::Image vortexImage{ vortex(image)};
        vortexImage.save("output/exercice17.png");
    }
}