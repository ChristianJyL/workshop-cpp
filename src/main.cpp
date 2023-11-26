#include <sil/sil.hpp>
#include <iostream>
#include "random.hpp"
#include <stdlib.h>
#include <math.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <utility>
#include <complex>
#include <algorithm>

void allGreen(sil::Image &image)
{
    // on parcourt tous les pixels de l'image
    for (glm::vec3 &color : image.pixels())
    {
        color.r = 0.f; // on met la composante rouge à 0
        color.b = 0.f; // on met la composante bleue à 0
    }
}

void inverseCanals(sil::Image &image)
{
    // on parcourt tous les pixels de l'image
    for (glm::vec3 &color : image.pixels())
    {
        std::swap(color.r, color.b); // on échange les composantes rouge et bleue
    }
}

void blackAndWhite(sil::Image &image)
{
    // on parcourt tous les pixels de l'image
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
        color = 1.f - color; // on inverse la couleur
    }
}

[[nodiscard]] sil::Image degrade()
{
    sil::Image image{300 /*width*/, 200 /*height*/};
    for (int x{0}; x < image.width(); x++)
    {
        float const value = static_cast<float>(x) / (image.width() - 1); // plus le x est grand plus value est grand et plus le pixel est clair
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
        if (random_value < 0.33) // 33% de chance d'avoir du bruit sur le pixel
        {
            color = glm::vec3(random_float(0.f, 1.f), random_float(0.f, 1.f), random_float(0.f, 1.f)); // on met une couleur aléatoire
        }
    }
}

[[nodiscard]] sil::Image rotate(sil::Image const &image)
{
    sil::Image rotateImage{image.height(), image.width()}; // on crée une image de la taille de l'image de départ mais on inverse la largeur et la hauteur

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
            newImage.pixel(x, y).r = image.pixel(final_x_backward, y).r;
            newImage.pixel(x, y).b = image.pixel(final_x_forward, y).b;
        }
    }
    return newImage;
}

// Fonction assombrissement ou éclaircissement
// si sombre est vrai alors on assombrit l'image sinon on l'éclaircit
void luminosity(sil::Image &image, bool const &sombre)
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

void disk(sil::Image &image, int const &rayon)
{
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if (std::pow(x - (image.width() / 2), 2) + std::pow(y - image.height() / 2, 2) < (std::pow(rayon, 2))) // si le pixel est dans le cercle
            {
                image.pixel(x, y) = glm::vec3(1); // on met le pixel en blanc
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
            if ((std::pow(x - center_x, 2) + std::pow(y - center_y, 2) < std::pow(rayon, 2) + thickess) && (std::pow(x - center_x, 2) + std::pow(y - center_y, 2) > std::pow(rayon, 2) - thickess)) // si le pixel est dans le cercle et qu'il est dans l'épaisseur du cercle
            {
                image.pixel(x, y) = glm::vec3(1); // on met le pixel en blanc
            }
        }
    }
}

void rosace(sil::Image &image, int const &rayon, int const &thinkness, int const &nbCircle)
{
    int const center_x = image.width() / 2;
    int const center_y = image.height() / 2;
    circle(image, rayon, thinkness, center_x, center_y);
    for (int i{0}; i < nbCircle; i++) // on fait nbCircle cercle
    {
        circle(image, rayon, thinkness, center_x + rayon * std::cos(i * (M_PI * 2 / nbCircle)), center_x + rayon * std::sin(i * (M_PI * 2 / nbCircle))); // chaque cercle est décalé de 2pi/nbCircle
    }
}

[[nodiscard]] sil::Image mosaique(sil::Image const &image, int const &repetition, bool const &mirror)
{
    int const mosaique_width = image.width() * repetition;
    int const mosaique_height = image.height() * repetition;
    sil::Image newImage{mosaique_width, mosaique_height};

    for (int x{0}; x < mosaique_width; x++)
    {
        for (int y{0}; y < mosaique_height; y++)
        {
            int const newX = x % image.width();  // on prend l'équivalent de x dans l'image de départ
            int const newY = y % image.height(); // on prend l'équivalent de y dans l'image de départ

            if (mirror && x / image.width() % 2 == 1) // si on veut faire un miroir et que l'on est sur une ligne paire
            {
                newImage.pixel(x, y) = image.pixel(image.width() - newX - 1, newY); // on prend le pixel de l'image de départ par la droite
                if (y / image.height() % 2 == 1)                                    // si on veut faire un miroir et que l'on est sur une colonne paire
                {
                    newImage.pixel(x, y) = image.pixel(image.width() - newX - 1, image.height() - newY - 1); // on prend le pixel de l'image de départ par la droite et par le bas
                }
            }
            else if (mirror && y / image.height() % 2 == 1) // si on veut faire un miroir et que l'on est sur une colonne paire
            {
                newImage.pixel(x, y) = image.pixel(newX, image.height() - newY - 1); // on prend le pixel de l'image de départ par le bas
            }
            else // sinon
            {
                newImage.pixel(x, y) = image.pixel(newX, newY); // on prend le pixel de l'image de départ
            }
        }
    }
    return newImage; // on retourne l'image car on ne modifie pas l'image de départ
}

// Première version de la fonction glitch qui parcourt tous les pixels de l'image
void glitch(sil::Image &image, int const &width, int const &height)
{
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float const random_value = random_float(0.f, 1.f);
            if (random_value < 0.001f) // pourcentage de chance d'avoir un glitch sur le pixel
            {
                int const random_width = random_int(1, width);                      // largeur du glitch
                int const random_height = random_int(1, height);                    // hauteur du glitch
                int const random_x = random_int(1, image.width() - random_width);   // position x du glitch
                int const random_y = random_int(1, image.height() - random_height); // position y du glitch

                // on fait l'échange de pixel du glitch
                for (int i{0}; i < random_width; i++)
                {
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

// Deuxième version de la fonction glitch qui fait un nombre de glitch aléatoire
void glitch2(sil::Image &image, int const &number_of_glitch, int const &width, int const &height)
{
    for (int i{0}; i < number_of_glitch; i++)
    {
        int const random_width = random_int(1, width);
        int const random_height = random_int(1, height);
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

[[nodiscard]] sil::Image fractale(int const &width, int const &height)
{
    sil::Image fractaleImage{width, width};
    for (int x{0}; x < fractaleImage.width(); x++)
    {
        for (int y{0}; y < fractaleImage.height(); y++)
        {
            std::complex<float> c{-2 + (static_cast<float>(x) / fractaleImage.width()) * 4, -2 + (static_cast<float>(y) / fractaleImage.height()) * 4};

            std::complex<float> z{0, 0};
            int i{0};
            do
            {
                z = z * z + c;
                i++;
            } while (std::abs(z) < 2 && i < 50);

            if (i >= 50)
            {
                fractaleImage.pixel(x, y) = glm::vec3(1);
            }
            else
            {
                fractaleImage.pixel(x, y) = glm::vec3(0 + i * 0.02);
            }
        }
    }
    return fractaleImage;
}

//----------------------------------------------------------------------------------------------------------------------------

glm::vec2 rotated(glm::vec2 point, glm::vec2 center_of_rotation, float angle)
{
    return glm::vec2{glm::rotate(glm::mat3{1.f}, angle) * glm::vec3{point - center_of_rotation, 0.f}} + center_of_rotation;
}

[[nodiscard]] sil::Image vortex(sil::Image const &image)
{
    sil::Image newImage{image};
    int const center_x{image.width() / 2};
    int const center_y{image.height() / 2};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float const angle = glm::distance(glm::vec2(x, y), glm::vec2(center_x, center_y)) / 10.f; // on calcule l'angle en fonction de la distance entre le pixel et le centre de l'image (plus le pixel est loin du centre plus l'angle est grand)
            glm::vec2 const new_position = rotated(glm::vec2(x, y), glm::vec2(center_x, center_y), angle);
            if (new_position.x < image.width() && new_position.y < image.height() && new_position.x > 0 && new_position.y > 0) // si le pixel est dans l'image
            {
                newImage.pixel(x, y) = image.pixel(new_position.x, new_position.y);
            }
            else // sinon on met le pixel en noir
            {
                newImage.pixel(x, y) = glm::vec3(0);
            }
        }
    }
    return newImage;
}

// tramage-----------------------------------------------------------------------------------------------------------------------------
void dithering(sil::Image &image)
{
    const int bayer_n = 4;
    float bayer_matrix_4x4[][bayer_n] = {
        {-0.5, 0, -0.375, 0.125},
        {0.25, -0.25, 0.375, -0.125},
        {-0.3125, 0.1875, -0.4375, 0.0625},
        {0.4375, -0.0625, 0.3125, -0.1875},
    };

    // on met l'image en noir et blanc
    blackAndWhite(image);

    for (int y{0}; y < image.height(); y++)
    {
        for (int x{0}; x < image.width(); x++)
        {
            float origin_color = image.pixel(x, y).r;                       // on récupère la couleur du pixel
            float bayer_value = bayer_matrix_4x4[y % bayer_n][x % bayer_n]; // on récupère la valeur de la matrice de bayer en fonction de la position du pixel
            float output_color = origin_color + (1 * bayer_value);          // on calcule la couleur du pixel en fonction de la couleur du pixel de départ et de la valeur de la matrice de bayer

            if (output_color < 1 / 2.f) // si la couleur est inférieur à 1/2
            {
                image.pixel(x, y) = glm::vec3(0); // on met le pixel en noir
            }
            else
            {
                image.pixel(x, y) = glm::vec3(1); // sinon on met le pixel en blanc
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------
float lum(int const &x, int const &y, sil::Image const &photo)
{
    float lum = (photo.pixel(x, y).r + photo.pixel(x, y).g + photo.pixel(x, y).b) / 3;
    return lum;
}

void normalisation(sil::Image &photo)
{
    float min_lum{1};
    float max_lum{0};
    for (int x{0}; x < photo.width(); x++)
    {
        for (int y{0}; y < photo.height(); y++)
        {
            if (lum(x, y, photo) < min_lum)
            {
                min_lum = lum(x, y, photo);
            }

            if (lum(x, y, photo) > max_lum)
            {
                max_lum = lum(x, y, photo);
            }
        }
    }

    for (int x{0}; x < photo.width(); x++)
    {
        for (int y{0}; y < photo.height(); y++)
        {
            photo.pixel(x, y) = (photo.pixel(x, y) - min_lum) / (max_lum - min_lum);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------

[[nodiscard]] sil::Image convolution(sil::Image const &image, std::vector<std::vector<float>> const &kernel)
{
    sil::Image blurImage{image};

    float kernelTotal{0};
    for (std::vector<float> line : kernel)
    {
        for (float number : line)
        {
            kernelTotal += number;
        }
    }

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            glm::vec3 color{0};
            int count{0};
            //int debug{-(static_cast<int>(kernel.size()) / 2)};
            for (int i{-(static_cast<int>(kernel.size()) / 2)}; i < static_cast<int>(kernel.size()); i++)
            {
                for (int j{-(static_cast<int>(kernel[0].size()) / 2)}; j < static_cast<int>(kernel[0].size()); j++)
                {
                    int coordX = x + i;
                    int coordY = y + j;
                    if (coordX < image.width() && coordX > 0 && coordY < image.height() && coordY > 0 && j + static_cast<int>(kernel[0].size()) / 2 < kernel[0].size() && i + static_cast<int>(kernel.size()) / 2 < kernel.size())
                    {
                        color += image.pixel(coordX, coordY) * kernel.at(i + static_cast<int>(kernel.size()) / 2).at(j + static_cast<int>(kernel[0].size()) / 2);
                    }
                    else
                    {
                        blurImage.pixel(x, y) += glm::vec3(0);
                    }
                }
            }
            if (kernelTotal != 0)
            {
                color = color / kernelTotal;
            }
            blurImage.pixel(x, y) = color;
        }
    }
    return blurImage;
}

std::vector<std::vector<float>> generateKernelBlurVertical(int const &size)
{
    std::vector<std::vector<float>> kernel(size, std::vector<float>(1, 1.f));
    return kernel;
}

std::vector<std::vector<float>> generateKernelBlurHorizontal(int const &size)
{
    std::vector<std::vector<float>> kernel(1, std::vector<float>(size, 1.f));
    return kernel;
}

[[nodiscard]] sil::Image differenceGaussian(sil::Image const &image, int const &sigma1, int const &sigma2)
{
    sil::Image gaussian1{image};
    sil::Image gaussian2{image};
    sil::Image result{image.width(), image.height()};

    gaussian1 = convolution(gaussian1, generateKernelBlurHorizontal(sigma1));
    gaussian1 = convolution(gaussian1, generateKernelBlurVertical(sigma1));

    gaussian2 = convolution(gaussian2, generateKernelBlurHorizontal(sigma2));
    gaussian2 = convolution(gaussian2, generateKernelBlurVertical(sigma2));

    float t = 0.5f;

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            result.pixel(x, y) = (1 - t) * gaussian1.pixel(x, y) - t * gaussian2.pixel(x, y);
        }
    }
    // tout les pixels qui sont noir on les met en blanc et les pixels en nuance de gris en noir
    for (glm::vec3 &color : result.pixels())
    {
        if (color.r < 0.009f)
        {
            color = glm::vec3(1);
        }
        else
        {
            color = glm::vec3(0);
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------

float brightness(glm::vec3 const &color)
{
    float value{(color.r + color.g + color.b) / 3};
    return value;
}

void tri(sil::Image &image)
{
    std::sort(image.pixels().begin(), image.pixels().end(), [](glm::vec3 const &color1, glm::vec3 const &color2)
              {
                  return brightness(color1) < brightness(color2); // Trie selon la luminosité des couleurs (NB : c'est à vous de coder la fonction `brightness`)
              });
}
//-------------------------------------------------------------------------------------------------------------

struct Region
{
    glm::vec3 average_color_region;
    float standart_deviation_region;
};

[[nodiscard]] sil::Image kuwahara(sil::Image const &image)
{
    sil::Image kuwaharaImage{image};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {

            std::vector<Region> listRegion;
            for (int offsetX : {-2, 1})
            {
                for (int offsetY : {-2, 1})
                {
                    glm::vec3 averageColor{};
                    float standardDeviation{};
                    for (int i{offsetX}; i < offsetX + 2; i++)
                    {
                        for (int j{offsetY}; j < offsetY + 2; j++)
                        {
                            if (x + i > 0 && y + j < image.height() && x + i < image.width() && y + j > 0)
                            {
                                averageColor += image.pixel(x + i, y + j);
                            }
                        }
                    }
                    averageColor = averageColor / 4.f;
                    for (int i{offsetX}; i < offsetX + 2; i++)
                    {
                        for (int j{offsetY}; j < offsetY + 2; j++)
                        {
                            if (x + i > 0 && y + j < image.height() && x + i < image.width() && y + j > 0)
                            {
                                standardDeviation += std::pow(glm::distance(image.pixel(x + i, y + j), averageColor), 2);
                            }
                        }
                    }

                    Region region;
                    region.average_color_region = averageColor;
                    region.standart_deviation_region = standardDeviation;
                    listRegion.push_back(region);
                }
            }

            // on cherche la région avec la plus petite déviation standard (on pourrait faire une fonction pour ça)
            float minStandardDeviation{listRegion[0].standart_deviation_region};
            glm::vec3 color{listRegion[0].average_color_region};
            for (Region regionOfList : listRegion)
            {
                if (regionOfList.standart_deviation_region < minStandardDeviation)
                {
                    minStandardDeviation = regionOfList.standart_deviation_region;
                    color = regionOfList.average_color_region;
                }
            }
            kuwaharaImage.pixel(x, y) = color;
        }
    }
    return kuwaharaImage;
}

// Essai rapide de la fonction diamond_square avec l'aide de la vidéo mais peu concluant-------------------------------------------------------------------------------
void square_step(int const &height_map_size, int const &chunk_size, int const &half, std::vector<std::vector<float>> &height_map, float const &roughness)
{
    for (int y{0}; y < height_map_size - 1; y += chunk_size)
    {
        for (int x{0}; x < height_map_size - 1; x += chunk_size)
        {
            if (x + chunk_size < height_map_size - 1 && y + chunk_size < height_map_size - 1)
            {
                float average = (height_map[y][x] + height_map[y][x + chunk_size] + height_map[y + chunk_size][x] + height_map[y + chunk_size][x + chunk_size]) / 4.f;

                height_map[y + half][x + half] = average + random_float(-roughness, roughness);
            }
        }
    }
}

void diamond_step(int const &height_map_size, int const &chunk_size, int const &half, std::vector<std::vector<float>> &height_map, float const &roughness)
{
    for (int y{0}; y < height_map_size; y += half)
    {
        for (int x{(y + half) % chunk_size}; x < height_map_size; x += chunk_size)
        {
            if (x - half > 0 && x + half < height_map_size - 1 && y - half > 0 && y + half < height_map_size - 1)
            {
                float average = (height_map[y - half][x] + height_map[y][x - half] + height_map[y][x + half] + height_map[y + half][x]) / 4.f;
                height_map[y][x] = average + random_float(-roughness, roughness);
            }
        }
    }
}

[[nodiscard]] sil::Image diamond_square(int const &n)
{
    int height_map_size = std::pow(n, 2) + 1;
    std::vector<std::vector<float>> height_map(height_map_size, std::vector<float>(height_map_size, 0.f));

    // initialisation des coins random
    height_map[0][0] = random_float(0.f, 1.f);
    height_map[0][height_map_size - 1] = random_float(0.f, 1.f);
    height_map[height_map_size - 1][0] = random_float(0.f, 1.f);
    height_map[height_map_size - 1][height_map_size - 1] = random_float(0.f, 1.f);

    int chunk_size = height_map_size - 1;
    float roughness = 2.f;

    while (chunk_size > 1)
    {
        float half = chunk_size / 2;
        square_step(height_map_size, chunk_size, half, height_map, roughness);
        diamond_step(height_map_size, chunk_size, half, height_map, roughness);
        chunk_size /= 2;
        roughness /= 2;
    }
    sil::Image height_map_image{height_map_size, height_map_size};
    for (int x{0}; x < height_map_size; x++)
    {
        for (int y{0}; y < height_map_size; y++)
        {
            height_map_image.pixel(x, y) = glm::vec3(height_map[x][y]);
        }
    }
    return height_map_image;
}

int main()
{
    sil::Image image{"images/logo.png"};
    sil::Image photo{"images/photo.jpg"};
    sil::Image photo_faible_constraste{"images/photo_faible_contraste.jpg"};

    {
        sil::Image copy{image};
        allGreen(copy);
        copy.save("output/allGreen.png");
    }

    {
        sil::Image copy{image};
        inverseCanals(copy);
        copy.save("output/inverseCanals.png");
    }

    {
        sil::Image copy{image};
        blackAndWhite(copy);
        copy.save("output/blackAndWhite.png");
    }

    {
        sil::Image copy{image};
        negatif(copy);
        copy.save("output/negatif.png");
    }
    {
        sil::Image degradeImage{degrade()};
        degradeImage.save("output/degrade.png");
    }
    {
        sil::Image copy{image};
        mirror(copy);
        copy.save("output/mirror.png");
    }
    {
        sil::Image copy{image};
        noise(copy);
        copy.save("output/noise.png");
    }
    {
        sil::Image rotateImage{rotate(image)};
        rotateImage.save("output/rotate.png");
    }
    {
        sil::Image rgbImage{RGBSplit(image)};
        rgbImage.save("output/RGBSplit.png");
    }
    {
        sil::Image copy{photo};
        luminosity(copy, false);
        copy.save("output/luminosity.png");
    }
    {
        sil::Image blackImage{500, 500};
        disk(blackImage, 100);
        blackImage.save("output/disk.png");
    }
    {
        sil::Image blackImage{500, 500};
        circle(blackImage, 100, 500, blackImage.width() / 2, blackImage.height() / 2);
        blackImage.save("output/circle.png");
    }
    {
        sil::Image blackImage{500, 500};
        rosace(blackImage, 100, 500, 6);
        blackImage.save("output/rosace.png");
    }
    {
        sil::Image mosaiqueImage{mosaique(image, 5, false)};
        mosaiqueImage.save("output/mosaique.png");
    }
    {
        sil::Image mosaiqueImage{mosaique(image, 5, true)};
        mosaiqueImage.save("output/mosaiqueMirroir.png");
    }
    {
        sil::Image copy{image};
        glitch(copy, 30, 10);
        copy.save("output/glitch1.png");
    }
    {
        sil::Image copy{image};
        glitch2(copy, 100, 30, 10);
        copy.save("output/glitch2.png");
    }

    {
        sil::Image fractaleImage{fractale(500, 500)};
        fractaleImage.save("output/fractale.png");
    }
    {
        sil::Image vortexImage{vortex(image)};
        vortexImage.save("output/vortex.png");
    }
    {
        sil::Image copy{photo};
        dithering(copy);
        copy.save("output/dithering.png");
    }
    {
        sil::Image copy{photo_faible_constraste};
        normalisation(copy);
        copy.save("output/normalisation.png");
    }
    {
        std::vector<std::vector<float>> kernelOutline = {
            {-1, -1, -1},
            {-1, 8, -1},
            {-1, -1, -1}};

        std::vector<std::vector<float>> kernelBlur3x3 = {{1, 2, 1},
                                                         {2, 4, 2},
                                                         {1, 2, 1}};

        std::vector<std::vector<float>> kernelEmboss = {{-2, -1, 1},
                                                        {-1, 1, 1},
                                                        {0, 1, 2}};

        std::vector<std::vector<float>> kernelSharpen = {{0, -1, 0},
                                                         {-1, 5, 1},
                                                         {0, -1, 0}};

        std::vector<std::vector<float>> kernelBlur5x5 = {{1, 4, 6, 4, 1},
                                                         {4, 16, 24, 16, 4},
                                                         {6, 24, 36, 24, 6},
                                                         {4, 16, 24, 16, 4},
                                                         {1, 4, 6, 4, 1}};

        sil::Image blurImage{convolution(image, kernelBlur5x5)};
        blurImage.save("output/convolution.png");
    }
    {
        // Separation matrix (more efficient)
        std::vector<std::vector<float>> kernelBlurHorizontal{{1, 1, 1}};
        std::vector<std::vector<float>> kernelBlurVertical{{1}, {1}, {1}};
        sil::Image blurImage{convolution(image, kernelBlurHorizontal)};
        blurImage = convolution(blurImage, kernelBlurVertical);
        blurImage.save("output/convolutionFiltreSepare.png");
    }
    {
        sil::Image imageDifference{differenceGaussian(photo, 1, 5)};
        imageDifference.save("output/differenceGaussian.png");
    }
    {
        sil::Image imageTri{photo};
        tri(imageTri);
        imageTri.save("output/tri.png");
    }
    {
        sil::Image imageKuwahara{kuwahara(photo)};
        imageKuwahara.save("output/kuwahara.png");
    }
    {
        sil::Image imageDiamondSquare{diamond_square(5)};
        imageDiamondSquare.save("output/diamond_square.png");
    }
    return 0;
}