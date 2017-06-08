#include "Mask.h"

#include<iostream>

const int Mask::NO_LABEL = -1;

Mask::Mask(BMP* bitmap) :
    mBitmap(bitmap)
{
}

void Mask::createLabels()
{
    mLabels.resize(getLength(), NO_LABEL);

    for (int j = 0; j < mBitmap->getHeight(); ++j)
    {
        for (int i = 0; i < mBitmap->getWidth(); ++i)
        {
            RGBPixel* pixel = mBitmap->get(i, j);
            if (pixel->isRed())
            {
                mLabels.at(mBitmap->getCoordinatesAsIndex(i, j)) = 0;
            }
            else if (pixel->isGreen())
            {
                mLabels.at(mBitmap->getCoordinatesAsIndex(i, j)) = 1;
            }
        }
    }
}

int Mask::getLabelAtIndex(int index)
{
    return mLabels.at(index);
}

int Mask::getLength()
{
    return mBitmap->getWidth() * mBitmap->getHeight();
}
