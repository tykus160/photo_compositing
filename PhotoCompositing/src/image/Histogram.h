#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "Image.h"

class Histogram
{
private:
    unsigned int* intensityR = new unsigned int[RGBPixel::MAX_VALUE + 1];
    unsigned int* intensityG = new unsigned int[RGBPixel::MAX_VALUE + 1];
    unsigned int* intensityB = new unsigned int[RGBPixel::MAX_VALUE + 1];

public:
    ~Histogram();
    void create(Image* image);
};

#endif // !__HISTOGRAM_H__
