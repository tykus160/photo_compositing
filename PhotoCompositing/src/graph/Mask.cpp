#include "Mask.h"

const int Mask::NO_LABEL = -1;

Mask::Mask(Image* image) :
    mImage(image)
{
    mWidth = mImage->getWidth();
    mHeight = mImage->getHeight();
    mLabels.resize(getLength(), NO_LABEL);
}

Mask::Mask(int width, int height) :
    mWidth(width), mHeight(height)
{
    mLabels.resize(getLength(), NO_LABEL);
}

void Mask::createLabels()
{
    if (mImage != nullptr)
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
    return getWidth() * getHeight();
}

int Mask::getWidth()
{
    return mWidth;
}

int Mask::getHeight()
{
    return mHeight;
}

void Mask::setLabelAtCoordinate(int x, int y, int label)
{
    mLabels.at(y * mWidth + x) = label;
}
