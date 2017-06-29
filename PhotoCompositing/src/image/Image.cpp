#include "Image.h"

Image::Image()
{
}

Image::Image(int width, int height) :
    width(width), height(height)
{
    pixels = new RGBPixel*[width * height];
    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            pixels[i] = new RGBPixel(i, j);
        }
    }
}

Image::~Image()
{
    for (int i = 0; i < width * height; ++i)
    {
        delete pixels[i];
    }
    delete[] pixels;
}

RGBPixel* Image::get(int x, int y)
{
    RGBPixel* result = nullptr;
    if (x < width && y < height)
    {
        result = pixels[y * width + x];
    }
    return result;
}

RGBPixel* Image::operator()(int x, int y)
{
    return get(x, y);
}

unsigned int Image::getCoordinatesAsIndex(int x, int y)
{
    return y * width + x;
}
