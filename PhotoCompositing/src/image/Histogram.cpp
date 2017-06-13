#include "Histogram.h"

Histogram::~Histogram()
{
    delete[] intensities;
}

void Histogram::create(Image* image)
{
    if (image != nullptr)
    {
        for (int y = 0; y < image->getHeight(); ++y)
        {
            for (int x = 0; x < image->getWidth(); ++x)
            {
                ++intensities[image->get(x, y)->getIntensity()];
            }
        }
    }
}
