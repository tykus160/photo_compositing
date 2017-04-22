#include "RGBPixel.h"

bool operator!=(const RGBPixel & a, const RGBPixel & b)
{
    return !(a == b);
}

bool operator==(const RGBPixel & a, const RGBPixel & b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}
