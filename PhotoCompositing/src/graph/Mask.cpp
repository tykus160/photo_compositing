#include "Mask.h"

#include<iostream>

const int Mask::NO_LABEL = -1;

Mask::Mask(Image* image) :
    mImage(image)
{
}

void Mask::createLabels()
{
    mLabels.resize(getLength(), NO_LABEL);

    for (int j = 0; j < mImage->getHeight(); ++j)
    {
        for (int i = 0; i < mImage->getWidth(); ++i)
        {
            RGBPixel* pixel = mImage->get(i, j);
            if (pixel->isRed())
            {
                mLabels.at(mImage->getCoordinatesAsIndex(i, j)) = 0;
            }
            else if (pixel->isGreen())
            {
                mLabels.at(mImage->getCoordinatesAsIndex(i, j)) = 1;
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
    return mImage->getWidth() * mImage->getHeight();
}
