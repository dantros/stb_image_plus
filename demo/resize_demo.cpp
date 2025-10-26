#include <stb_image_plus.h>
#include <iostream>
#include <algorithm>

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage: resize_demo input_image width height output_prefix" << std::endl;
        return 1;
    }

    std::string input_filename(argv[1]);
    std::size_t width = atoi(argv[2]);
    std::size_t height = atoi(argv[3]);
    std::string output_filename(argv[4]);

    std::cout << "Parsing inputs:" << std::endl;
    std::cout << input_filename << std::endl;
    std::cout << width << std::endl;
    std::cout << height << std::endl;
    std::cout << output_filename << std::endl;
    std::cout << std::endl;

    stb_image_plus::ImageData3 image(input_filename);
    if (image.isValid())
        std::cout << "Image loaded!" << std::endl;
    else
    {
        std::cout << "Problems loading the image." << std::endl;
        return 1;
    }

    if (width == 0 or height == 0)
    {
        std::cout << "Invalid new size." << std::endl;
        return 1;
    }

    {
        stb_image_plus::ImageData3 resizedImage = image.resize(width, height);
        resizedImage.write(output_filename + ".jpg");
    }
    {
        unsigned int newWidth = image.width() / 2;
        stb_image_plus::ImageData3 resizedImage = image.resizeToWidth(newWidth);
        resizedImage.write(output_filename + "_halfWidth.jpg");
    }
    {
        unsigned int newHeight = image.height() * 2;
        stb_image_plus::ImageData3 resizedImage = image.resizeToHeight(newHeight);
        resizedImage.write(output_filename + "_doubleHeight.jpg");
    }
    {
        std::span<stb_image_plus::ImageData3::Pixel> pixels = image.pixelSpan();

        for (stb_image_plus::ImageData3::Pixel& pixel : pixels)
        {
            pixel[0] = std::min(static_cast<int>(pixel[0] * 1.2f), 255);
            pixel[1] *= 0.5f;
            pixel[2] *= 0.5f;
        }

        image.write(output_filename + "_halfColor.jpg");
    }
}