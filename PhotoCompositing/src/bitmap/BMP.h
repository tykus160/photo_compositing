#ifndef __BMP_H__
#define __BMP_H__

#include "RGBAPixel.h"

class BMP
{
private:
    RGBAPixel** pixels;
    int width;
    int height;
    int length;
public:
    BMP(char* filename);
    ~BMP();
    RGBAPixel* operator()(int x, int y);
};

#endif