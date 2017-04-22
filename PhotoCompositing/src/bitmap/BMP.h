#ifndef __BMP_H__
#define __BMP_H__

#include "RGBPixel.h"

class BMP
{
private:
    RGBPixel** pixels;
    int width;
    int height;
    int length;
public:
    BMP(char* filename);
    ~BMP();
    RGBPixel* operator()(int x, int y);
};

#endif