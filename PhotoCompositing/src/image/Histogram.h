#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "Image.h"

class Histogram
{
private:
    unsigned int* intensities = new unsigned int[RGBPixel::MAX_VALUE + 1];

public:
    ~Histogram();
    void create(Image* image);
};

#endif // !__HISTOGRAM_H__
