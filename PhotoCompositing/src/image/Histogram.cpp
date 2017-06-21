#include "Histogram.h"

Histogram::~Histogram()
{
    delete[] intensityR;
    delete[] intensityG;
    delete[] intensityB;
}

void Histogram::create(Image* image)
{
    if (image != nullptr)
    {
        for (int y = 0; y < image->getHeight(); ++y)
        {
            for (int x = 0; x < image->getWidth(); ++x)
            {
                ++intensityR[image->get(x, y)->r];
                ++intensityG[image->get(x, y)->g];
                ++intensityB[image->get(x, y)->b];
            }
        }
    }
}
