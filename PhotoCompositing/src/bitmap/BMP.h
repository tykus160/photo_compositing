#ifndef __BMP_H__
#define __BMP_H__

#include "RGBPixel.h"

class BMP
{
private:
    RGBPixel** pixels;
    int width;
    int height;
public:
    BMP(char* filename);

    ~BMP();

    void saveToFile(char* filename);

    RGBPixel* get(int x, int y);

    RGBPixel* operator()(int x, int y);

    int getCoordinatesAsIndex(int x, int y);

    int getWidth()
    {
        return width; 
    }

    int getHeight()
    {
        return height;
    }
};

#endif
