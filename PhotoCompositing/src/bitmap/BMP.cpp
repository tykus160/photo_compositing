#include "BMP.h"
#include <fstream>

BMP::BMP(char* filename)
{
    std::ifstream* file = new std::ifstream();
    file->open(filename, std::ios::binary);
    
    length = width * height;
    pixels = new RGBAPixel*[length];
    for (int i = 0; i < length; ++i)
    {

    }

    file->close();
    delete file;
}

BMP::~BMP()
{
    for (int i = 0; i < length; ++i)
    {
        delete[] pixels[i];
    }
    delete [] pixels;
}

RGBAPixel* BMP::operator()(int x, int y)
{
    RGBAPixel* result = nullptr;
    if (x < width && y < height)
    {
        result = pixels[y * height + x];
    }
    return result;
}
