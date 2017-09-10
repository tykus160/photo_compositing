#include "Mask.h"

#include <cstdlib>
#include <map>
#include <iostream>

#include "../image/bitmap/BMP.h"

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

Mask::Mask(const Mask& other)
{
    mWidth = other.mWidth;
    mHeight = other.mHeight;
    mLabelCounter = other.mLabelCounter;
    mLabels = other.mLabels;
    // do not copy address of image, you shouldn't need this
}

Mask::~Mask()
{
    mLabels.clear();
}

void Mask::createLabels()
{
    if (mImage != nullptr)
    {
        RGBPixel px;
        px.r = 255;
        px.g = 255;
        px.b = 255;
        std::map<unsigned int, int> mColors; // temporal storage for all colors in mask
        mLabelCounter = 0;
        mColors[px.toHex()] = NO_LABEL; // add white color as default

        for (int j = 0; j < mImage->getHeight(); ++j)
        {
            for (int i = 0; i < mImage->getWidth(); ++i)
            {
                unsigned int hex = mImage->get(i, j)->toHex();

                auto it = mColors.find(hex);
                if (it == mColors.end())
                {
                    //element not found
                    mLabels.at(mImage->getCoordinatesAsIndex(i, j)) = mLabelCounter;
                    mColors[hex] = mLabelCounter++;
                    std::cout << "coord (" << i << "," << j << "), hex = " << hex << std::endl;
                }
                else
                {
                    mLabels.at(mImage->getCoordinatesAsIndex(i, j)) = it->second;
                }
            }
        }
        std::cout << "Found colors: " << mLabelCounter << std::endl;
        mColors.clear();
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

void Mask::fill()
{
    for (int j = 0; j < mImage->getHeight(); ++j)
    {
        for (int i = 0; i < mImage->getWidth(); ++i)
        {
            if (getLabelAtCoordinate(i, j) == NO_LABEL)
            {
                setLabelAtCoordinate(i, j, std::rand() % mLabelCounter);
            }
        }
    }
}

int Mask::getNumberOfLabels()
{
    return mLabelCounter;
}
