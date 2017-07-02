#include "Image.h"

Image::Image()
{
}

Image::Image(int width, int height) :
    width(width), height(height)
{
    pixels = new RGBPixel*[width * height];
}

Image::~Image()
{
    for (int i = 0; i < width * height; ++i)
    {
        delete pixels[i];
    }
    delete[] pixels;
}

void Image::set(int x, int y, RGBPixel* px)
{
    pixels[getCoordinatesAsIndex(x, y)] = px;
}

RGBPixel* Image::get(int x, int y)
{
    RGBPixel* result = nullptr;
    if (x < width && y < height && x >= 0 && y >= 0)
    {
        result = pixels[getCoordinatesAsIndex(x,y)];
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
