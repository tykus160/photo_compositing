/**
 * Base Image class, implements most of needed functionality.
 * Children of this should implement reading file (via constructor) and saving.
 *
 * @author W.Tyczynski
 */
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "rgbpixel/RGBPixel.h"

class Image
{
protected:
    RGBPixel** pixels;
    int width;
    int height;

public:
    Image();
    
    Image(int width, int height);

    ~Image();

    virtual void saveToFile(char* filename) = 0;

    RGBPixel* get(int x, int y);

    void set(int x, int y, RGBPixel* px);

    RGBPixel* operator()(int x, int y);

    unsigned int getCoordinatesAsIndex(int x, int y);

    int getWidth()
    {
        return width;
    }

    int getHeight()
    {
        return height;
    }
};

#endif // !__IMAGE_H__
