#ifndef __COST_FUNCTIONS_H__
#define __COST_FUNCTIONS_H__

#include "../image/rgbpixel/RGBPixel.h"
#include "Mask.h"

/** Pointer for CostFunctions methods */
typedef double(*CostFunction)(RGBPixel&, RGBPixel&, Mask*);

namespace CostFunctions
{
    double distance(RGBPixel& a, RGBPixel& b, Mask* mask);

    double labeling(RGBPixel& a, RGBPixel& b, Mask* mask);
}

#endif // !__COST_FUNCTIONS_H__
