#include "Mask.h"

Mask::Mask(BMP* bitmap) :
    mBitmap(bitmap)
{
}

void Mask::createLabels()
{
    RGBPixel* temp = new RGBPixel[2];
    temp[0].b = 2;
    mFields.push_back(temp);
}
