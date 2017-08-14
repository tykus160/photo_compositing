#include "Mask.h"
#include "../image/bitmap/BMP.h"

const int Mask::NO_LABEL = -1;

Mask::Mask(Image* image) :
    mImage(image)
{
    mWidth = mImage->getWidth();
    mHeight = mImage->getHeight();
    mLabels.resize(getLength(), 0);
}

Mask::Mask(int width, int height) :
    mWidth(width), mHeight(height)
{
    mLabels.resize(getLength(), 0);
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

void Mask::saveToImage(std::string filename)
{
    Image* image = new BMP(mWidth, mHeight);
    for (int y = 0; y < mHeight; ++y)
    {
        for (int x = 0; x < mWidth; ++x)
        {
            RGBPixel* pixel = new RGBPixel;
            if (getLabelAtCoordinate(x, y) == 0)
            {
                pixel->r = 255;
                pixel->g = 255;
                pixel->b = 255;
            }
            else if (getLabelAtCoordinate(x, y) == 1)
            {
                pixel->r = 0;
                pixel->g = 255;
                pixel->b = 0;
            }
            image->set(x, y, pixel);
        }
    }
    image->saveToFile(filename);
    delete image;
}
