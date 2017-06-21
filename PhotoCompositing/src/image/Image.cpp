#include "Image.h"

Image::Image()
{
}

Image::Image(int width, int height) :
    width(width), height(height)
{
    const int length = width * height;
    pixels = new RGBPixel*[length];
    for (int i = 0; i < length; ++i)
    {
        pixels[i] = new RGBPixel;
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
