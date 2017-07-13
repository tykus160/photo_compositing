#include "CostFunctions.h"

double CostFunctions::distance(RGBPixel& a, RGBPixel& b, Mask* mask)
{
    return a.distance(b);
}

double CostFunctions::labeling(RGBPixel& a, RGBPixel& b, Mask* mask)
{
    return mask->getLabelAtCoordinate(a.x, a.y) == mask->getLabelAtCoordinate(b.x, b.y) ? 0 : 1;
}
