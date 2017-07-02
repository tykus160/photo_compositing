#include "Mask.h"

const int Mask::NO_LABEL = -1;

Mask::Mask(Image* image) :
    mImage(image)
{
    mLabels.resize(getLength(), 0);
}

void Mask::createLabels()
{
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

int Mask::getLabelAtCoordinate(int x, int y)
{
    return getLabelAtIndex(y * getWidth() + x);
}

int Mask::getLength()
{
    return mImage->getWidth() * mImage->getHeight();
}

int Mask::getWidth()
{
    return mImage->getWidth();
}

int Mask::getHeight()
{
    return mImage->getHeight();
}

void Mask::setLabelAtCoordinate(int x, int y, int label)
{
    mLabels.at(mImage->getCoordinatesAsIndex(x, y)) = label;
}
