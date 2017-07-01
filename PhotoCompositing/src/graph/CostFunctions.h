#ifndef __COST_FUNCTIONS_H__
#define __COST_FUNCTIONS_H__

#include "../image/rgbpixel/RGBPixel.h"
#include "Mask.h"

namespace CostFunctions
{
    double distance(RGBPixel& a, RGBPixel& b, Mask* mask)
    {
        return a.distance(b);
    }

    int labeling(RGBPixel& a, RGBPixel& b, Mask* mask)
    {
        return mask->getLabelAtCoordinate(a.x, a.y) == mask->getLabelAtCoordinate(a.x, a.y) ? 0 : 1;
    }
}

#endif // !__COST_FUNCTIONS_H__
